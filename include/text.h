#pragma once 
#include "OIB.h"

#define TEXT_INLINE_CAPACITY 32

typedef struct {
	char inlineString[TEXT_INLINE_CAPACITY];
	//if inline string is not big enough heap allocate to string
	char *string;
	int lines;
	int width;
} Text;

Text *makeText(char *string, int width);
void fillText(Text *txt, char *string);
void freeText(Text *txt);
