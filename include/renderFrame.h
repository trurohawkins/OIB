#ifndef RENDERFRAME
#define RENDERFRAME
#include <stdint.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdio.h>

#define RENDER_BUFFER_SIZE 256
typedef struct {
	int layer;
	int type;
	int index;
	int cmd;

	uint8_t data[RENDER_BUFFER_SIZE];
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
	int lastRender;
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

void windowResizeCallback(int sig);


typedef struct {
	int x;
	int y;
} Pos;

typedef struct {
	uint8_t vals[3];
} Color;

typedef struct {
	Pos pos;
	Color color;
} PosColor;

Color mixColors(Color *a, Color *b, float t);
#endif
