#include "color.h"

Color mixColors(Color *a, Color *b, float t) {
	Color ret = {
		.rgb = {0, 0, 0},
	};
	if (a || b) {
		if (!a) {
			return *b;
		} else if (!b) {
			return *a;
		}
		for (int i = 0; i < 3; i++) {
			ret.rgb[i] = lerp(a->rgb[i], b->rgb[i], t);
		}
	}
	return ret;
}

Color tintColor(Color c, Tint t) {
	for (int i = 0; i < 3; i++) {
		c.rgb[i] = applyBrightness(c.rgb[i], t.value);
		c.rgb[i] = blendColorVal(c.rgb[i], t.rgb[i], t.strength);
	}
	return c;
}

uint8_t applyBrightness(uint8_t c, int8_t value) {
	if (value < 0) {
		return (uint8_t)(c * (127 + value) / 127);
	} else {
		return (uint8_t)(c + (((255 - c) * value) / 127));
	}
}

uint8_t blendColorVal(uint8_t a, uint8_t b, uint8_t strength) {
	uint8_t blended = (a * (255 - strength) + b * strength) / 255;
	return blended;
}

Tint neutralTint() {
	Tint t = {
		.rgb = {255, 255, 255},
		.value = 0,
		.strength = 0,
	};
	return t;
}

