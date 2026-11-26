#include "text.h"

Text *makeText(char *string, int width) {
	Text *txt = calloc(1, sizeof(Text));
	txt->width = width;
	fillText(txt, string);
	return txt;
}

int countLines(char *string, int width) {
	int len = strlen(string);
	width = min(width, len);
	int xp = 0;
	int lines = 1;
	for (int i = 0; i < len; i++) {
		xp++;
		if (xp % width == 0 || string[i] == '\n') {
			if (i + 1 < len) {
				lines++;
				xp = 0;
			}
		}
	}
	return lines;
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
		txt->lines = countLines(string, txt->width);
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

