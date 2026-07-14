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
		frames[currentFrame].num = 0;
		return true;
	} else {
		return false;
	}
}

void addRenderCommand(RenderCommand reco) {
	if (currentFrame >= 0 && currentFrame < NUM_FRAMES) {
		if (frames[currentFrame].num < RENDER_COMMAND_MAX) {
			frames[currentFrame].queue[frames[currentFrame].num] = reco;
			frames[currentFrame].num++;;
		}
	}
}

void sendRenderFrame() {
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
