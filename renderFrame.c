#include "MoltnCore.h"
#include "renderFrame.h"

RenderFrame frames[NUM_FRAMES];
atomic_int renderWriteIndex;
atomic_int renderReadIndex;
atomic_int renderActiveIndex;

atomic_int newRender = 0;
atomic_int windowResized = 0;

int screenX = 0;
int screenY = 0;

void freeRenderFrames() {
	for (int i = 0; i < NUM_FRAMES; i++) {
		free(frames[i].content);
	}
}

void makeRenderFrames(int width, int height) {
	//printf("making renderframes %i, %i\n", width, height);
	for (int i = 0; i < NUM_FRAMES; i++) {
		frames[i].width = width;
		frames[i].height = height;
		frames[i].content = calloc(width * height, sizeof(Glyph));
	}
	screenX = width;
	screenY = height;
}

int findFreeFrame() {
	int read = atomic_load_explicit(&renderReadIndex, memory_order_acquire);
	int active = atomic_load_explicit(&renderActiveIndex, memory_order_acquire);
	for (int i = 0; i < NUM_FRAMES; i++) {
		if (i != read && i != active) {
			return i;
		}
	}
	return -1;
}

void renderObjects(linkedList *renders) {
	int newFrame = findFreeFrame();
	if (newFrame == -1) {
		return;
	}
	//populate B
	Glyph border = {
		.fr = 128,
		.fg = 1,
		.fb = 1,
		.br = 1,
		.bg = 1,
		.bb = 1,

		.symbol = '/'
	};
	RenderFrame *frame = frames + newFrame;
	for (int i = 0; i < frame->width * frame->height; i++) {
		frame->content[i] = border;
	}
	
	rend empty = {
		.symbol = ' ',
		.r = 0,
		.g = 0,
		.b = 0,
		.priority = -1,
	};
	rend *figures = calloc(frame->width * frame->height, sizeof(rend));
	rend *grounds = calloc(frame->width * frame->height, sizeof(rend));
	for (int i = 0; i < frame->width * frame->height; i++) {
		figures[i] = empty;
		grounds[i] = empty;
	}
	linkedList *cur = renders;
	while (cur) {
		RenderObject *rob = cur->data;
		if (rob) {
			linkedList *commands = rob->render(rob->data);
			linkedList *curCom = commands;
			while (curCom) {
				RenderCommand *reco = curCom->data;	
				int i = reco->screenPos[1] * frame->width + reco->screenPos[0];
				if (i >= 0 && i < frame->width * frame->height) {
					if (reco->sigil.figure) {
						if (figures[i].priority < reco->sigil.priority) {
							figures[i] = reco->sigil;
						}
					} else {
						if (grounds[i].priority < reco->sigil.priority) {
							grounds[i] = reco->sigil;
						}
					}
				}
				curCom = curCom->next;
			}
			freeList(&commands);
		}
		cur = cur->next;
	}
	for (int i = 0; i < frame->height * frame->width; i++) {
		Glyph *g = &frame->content[i];
		g->symbol = figures[i].symbol;
		g->fr = figures[i].r;
		g->fg = figures[i].g;
		g->fb = figures[i].b;

		g->br = grounds[i].r;
		g->bg = grounds[i].g;
		g->bb = grounds[i].b;
	}
	free(figures);
	free(grounds);
	atomic_store_explicit(&renderWriteIndex, newFrame, memory_order_release);
	//signal redraw
	setNewRender();
}

void renderFrame(Glyph *glyphs, int *poses, int glyphCount) {
	// atomic load next frame
	//int next = atomic_load_explicit(&renderReadIndex, memory_order_acquire);
	//int newFrame = (next + 1) % NUM_FRAMES;
	int newFrame = findFreeFrame();
	if (newFrame == -1) {
		return;
	}
	//populate B
	Glyph border = {
		.fr = 128,
		.fg = 1,
		.fb = 1,
		.br = 1,
		.bg = 1,
		.bb = 1,

		.symbol = '/'
	};
	RenderFrame *frame = frames + newFrame;
	for (int i = 0; i < frame->width * frame->height; i++) {
		frame->content[i] = border;
	}
	for (int i = 0; i < glyphCount; i++) {
		int s = poses[i];
		if (s >= 0 && s < frame->width * frame->height) {
			frame->content[s] = glyphs[i];
		}
	}

	atomic_store_explicit(&renderWriteIndex, newFrame, memory_order_release);
	//signal redraw
	setNewRender();
}

void setNewRender() {
	atomic_store_explicit(&newRender, 1, memory_order_release);
	uint64_t v = 1;
	if (write(outputPoll.handler.fd, &v, sizeof(v)) == -1) {
		perror("write outpoll fd");
	}
}

void windowResizeCallback(int sig) {
	atomic_store_explicit(&windowResized, 1, memory_order_release);
}
