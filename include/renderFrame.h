#ifndef RENDERFRAME
#define RENDERFRAME
#include <stdint.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdio.h>

typedef struct {
	int screenPos[2];
	int sigil;
	uint8_t r;
	uint8_t g;
	uint8_t b;
	int layer;
} RenderCommand;

#define RENDER_COMMAND_MAX 10000
#define MAX_RENDER_LAYERS 16
typedef struct {
	int num;
	RenderCommand queue[RENDER_COMMAND_MAX];
} RenderFrame;

typedef struct {
	void *(*render)(void*);
	void *data;
} RenderObject;

#define NUM_FRAMES 3
extern RenderFrame frames[NUM_FRAMES];

extern atomic_int renderWriteIndex;
extern atomic_int renderReadIndex;
extern atomic_int renderActiveIndex;

extern atomic_int newRender;

extern int screenX;
extern int screenY;

void updateScreenSize(int width, int height); 

bool startRendering();
void addRenderCommand(RenderCommand reco);
void sortRenderCommands();
void sendRenderFrame();

void setNewRender();
void windowResizeCallback(int sig);

#endif
