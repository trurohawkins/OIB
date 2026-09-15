#include "text.h"

Text *makeText(char *string, int width) {
	Text *txt = calloc(1, sizeof(Text));
	txt->width = width;
	fillText(txt, string);
	return txt;
}

void fillText(Text *txt, char *string) {
	int len = strlen(string);
	if (len > 0) {
		if (txt->string) {
			free(txt->string);
		}
		if (len > TEXT_INLINE_CAPACITY) {
			txt->string = calloc(len+1, sizeof(char));
			memcpy(txt->string, string, len);
			memset(txt->inlineString, 0, TEXT_INLINE_CAPACITY);
		} else {
			txt->string = 0;
			memcpy(txt->inlineString, string, len);
		}
		int width = min(txt->width, len);
		int xp = 0;
		txt->lines = 1;
		for (int i = 0; i < len; i++) {
			xp++;
			if (xp % width == 0 || string[i] == '\n') {
				if (i + 1 < len) {
					txt->lines++;
					xp = 0;
				}
			}
		}
	} else {
		txt->lines = 1;
	}
}

void freeText(Text *txt) {
	if (txt->string) {
		free(txt->string);
	}
	free(txt);
}

