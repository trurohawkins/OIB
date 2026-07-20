#include "MoltnCore.h"
#include "renderFrame.h"

RenderFrame frames[NUM_FRAMES];
RenderFrame curCommands;
int counts[MAX_RENDER_LAYERS+1];

atomic_int renderWriteIndex;
atomic_int renderReadIndex;
atomic_int renderActiveIndex;

atomic_int newRender = 0;
atomic_int windowResized = 0;

int screenX = 0;
int screenY = 0;
int currentFrame = -1;

void updateScreenSize(int width, int height) {
	screenX = width;
	screenY = height;
	//makeRenderFrame();
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

bool startRendering() {
	currentFrame = findFreeFrame();
	if (currentFrame >= 0 && currentFrame < NUM_FRAMES) {
		curCommands.num = 0;
		return true;
	} else {
		return false;
	}
}

void addRenderCommand(RenderCommand reco) {
	if (currentFrame >= 0 && currentFrame < NUM_FRAMES) {
		if (curCommands.num < RENDER_COMMAND_MAX) {
			curCommands.queue[curCommands.num] = reco;
			curCommands.num++;
		}
	}
}

void sortRenderCommands() {
	if (currentFrame >= 0 && currentFrame < NUM_FRAMES) {
		memset(counts, 0, sizeof(int) * MAX_RENDER_LAYERS);
		for (int i = 0; i < curCommands.num; i++) {
			RenderCommand reco = curCommands.queue[i];
			counts[reco.layer]++;
		}
		//char buff[100];
		// compute prefix sum
		for (int i = 1; i <= MAX_RENDER_LAYERS; i++) {
			counts[i] += counts[i-1];
			//sprintf(buff, "counts[%d] = %d\n", i, counts[i]);
			//debugWrite(buff);
		}
		RenderFrame *frame = &frames[currentFrame];
		frame->num = curCommands.num;
		for (int i = frame->num - 1; i >= 0; i--) {
			RenderCommand reco = curCommands.queue[i];
			frame->queue[counts[reco.layer]-1] = curCommands.queue[i];
			counts[reco.layer]--;
		}
	}
}

void sendRenderFrame() {
	sortRenderCommands();
	atomic_store_explicit(&renderWriteIndex, currentFrame, memory_order_release);
	//signal redraw
	setNewRender();
	currentFrame = -1;
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
