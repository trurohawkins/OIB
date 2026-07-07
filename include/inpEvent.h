#ifndef INPEVENT
#define INPEVENT
typedef struct {
	uint8_t key;
	bool val; // 1: pressed 0: released
} KeyEvent;
#endif
