#ifndef RENDERFRAME
#define RENDERFRAME
#include <stdint.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdio.h>

typedef struct {
	char symbol;

	uint8_t fr;
	uint8_t fg;
	uint8_t fb;

	uint8_t br;
	uint8_t bg;
	uint8_t bb;
} Glyph;

typedef struct {
	bool figure;
	char symbol;
	uint8_t r;
	uint8_t g;
	uint8_t b;

	int priority;
} rend;

typedef struct {
 int screenPos[2];
 rend sigil;
} RenderCommand;

typedef struct {
	void *(*render)(void*);
	void *data;
} RenderObject;

typedef struct {
	int width;
	int height;
	Glyph *content;
} RenderFrame;

#define NUM_FRAMES 3
extern RenderFrame frames[NUM_FRAMES];
extern atomic_int renderWriteIndex;
extern atomic_int renderReadIndex;
extern atomic_int renderActiveIndex;

extern atomic_int newRender;

extern int screenX;
extern int screenY;

void freeRenderFrames();
void makeRenderFrames(int width, int height);
void renderObjects(linkedList *renders);
void renderFrame(Glyph *glyphs, int *poses, int num);

void setNewRender();
void windowResizeCallback(int sig);

#endif
