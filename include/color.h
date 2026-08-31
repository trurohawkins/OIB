#pragma once
#include <stdint.h>
#include "MoltnCore.h"

typedef struct {
	uint8_t rgb[3];
} Color;

typedef struct {
	uint8_t rgb[3]; // tint color 0 - 255
	int8_t value;		//-127 darkens, 0 = neutral, +127 = lighten
	uint8_t strength; // 0 - 255
} Tint;

Color mixColors(Color *a, Color *b, float t);
uint8_t applyBrightness(uint8_t c, int8_t value);
uint8_t blendColorVal(uint8_t a, uint8_t b, uint8_t strength);
Color tintColor(Color c, Tint t);
Tint neutralTint();

