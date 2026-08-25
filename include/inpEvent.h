#ifndef INPEVENT
#define INPEVENT
typedef uint16_t KeyCode;

enum {
    /* ASCII control characters */
    KEY_NUL         =   0,
    KEY_CTRL_A      =   1,
    KEY_CTRL_B      =   2,
    KEY_CTRL_C      =   3,
    KEY_CTRL_D      =   4,
    KEY_CTRL_E      =   5,
    KEY_CTRL_F      =   6,
    KEY_CTRL_G      =   7,
    KEY_BACKSPACE   =   8,
    KEY_TAB         =   9,
    KEY_LF          =  10,
    KEY_CTRL_K      =  11,
    KEY_CTRL_L      =  12,
    KEY_ENTER       =  13,
    KEY_CTRL_N      =  14,
    KEY_CTRL_O      =  15,
    KEY_CTRL_P      =  16,
    KEY_CTRL_Q      =  17,
    KEY_CTRL_R      =  18,
    KEY_CTRL_S      =  19,
    KEY_CTRL_T      =  20,
    KEY_CTRL_U      =  21,
    KEY_CTRL_V      =  22,
    KEY_CTRL_W      =  23,
    KEY_CTRL_X      =  24,
    KEY_CTRL_Y      =  25,
    KEY_CTRL_Z      =  26,
    KEY_ESCAPE      =  27,
    KEY_FS          =  28,
    KEY_GS          =  29,
    KEY_RS          =  30,
    KEY_US          =  31,

    /* Printable ASCII */
    KEY_SPACE       = ' ',

    /* Everything else is just ASCII */
    /* '!' through '~' */

    /* Extended keys start here */
    KEY_DELETE      = 128,

    KEY_UP,
    KEY_LEFT,
    KEY_DOWN,
    KEY_RIGHT,

    KEY_HOME,
    KEY_END,

    KEY_PAGE_UP,
    KEY_PAGE_DOWN,

    KEY_INSERT,

    KEY_F1,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,
    KEY_F6,
    KEY_F7,
    KEY_F8,
    KEY_F9,
    KEY_F10,
    KEY_F11,
    KEY_F12,

    KEY_PRINT_SCREEN,
    KEY_SCROLL_LOCK,
    KEY_PAUSE,

    KEY_CAPS_LOCK,

    KEY_NUM_LOCK,

    KEY_MENU,

    KEY_LSHIFT,
    KEY_RSHIFT,

    KEY_LCTRL,
    KEY_RCTRL,

    KEY_LALT,
    KEY_RALT,

    KEY_LSUPER,
    KEY_RSUPER,

};

typedef struct {
	KeyCode key;
	bool val; // 1: pressed 0: released
} KeyEvent;

typedef struct {
    const char *seq;
    KeyCode key;
} KeySequence;

#define KEY_SEQUENCES 22

static const KeySequence sequences[] = {
    { "\x1b[A", KEY_UP },
    { "\x1b[B", KEY_DOWN },
    { "\x1b[C", KEY_RIGHT },
    { "\x1b[D", KEY_LEFT },

    { "\x1b[H", KEY_HOME },
    { "\x1b[F", KEY_END },

    { "\x1b[2~", KEY_INSERT },
    { "\x1b[3~", KEY_DELETE },
    { "\x1b[5~", KEY_PAGE_UP },
    { "\x1b[6~", KEY_PAGE_DOWN },

    { "\x1bOP", KEY_F1 },
    { "\x1bOQ", KEY_F2 },
    { "\x1bOR", KEY_F3 },
    { "\x1bOS", KEY_F4 },

    { "\x1b[15~", KEY_F5 },
    { "\x1b[17~", KEY_F6 },
    { "\x1b[18~", KEY_F7 },
    { "\x1b[19~", KEY_F8 },
    { "\x1b[20~", KEY_F9 },
    { "\x1b[21~", KEY_F10 },
    { "\x1b[23~", KEY_F11 },
    { "\x1b[24~", KEY_F12 },
};
#endif
