#include "arch/keyboard.h"
#include "drivers/stdin.h"
#include "drivers/keycode.h"
#include "lamune/types.h"
#include "lamune/assert.h"

# define KEYBOARD_FLAG_SHIFT 0x1
# define KEYBOARD_FLAG_CAPSLOCK 0x2

struct ascii_field
{
	uint32_t flags:8;
	uint32_t upper:8;
	uint32_t lower:8;
} printable[] = {
	[KEY_1] = {KEYBOARD_FLAG_SHIFT, '!', '1'},
	[KEY_2] = {KEYBOARD_FLAG_SHIFT, '@', '2'},
	[KEY_3] = {KEYBOARD_FLAG_SHIFT, '#', '3'},
	[KEY_4] = {KEYBOARD_FLAG_SHIFT, '$', '4'},
	[KEY_5] = {KEYBOARD_FLAG_SHIFT, '%', '5'},
	[KEY_6] = {KEYBOARD_FLAG_SHIFT, '^', '6'},
	[KEY_7] = {KEYBOARD_FLAG_SHIFT, '&', '7'},
	[KEY_8] = {KEYBOARD_FLAG_SHIFT, '*', '8'},
	[KEY_9] = {KEYBOARD_FLAG_SHIFT, '(', '9'},
	[KEY_0] = {KEYBOARD_FLAG_SHIFT, ')', '0'},

	[KEY_MINUS] = {KEYBOARD_FLAG_SHIFT, '_', '-'},
	[KEY_EQUAL] = {KEYBOARD_FLAG_SHIFT, '+', '='},
	[KEY_BACKSPACE] = {0, 0, 0x8},
	[KEY_TAB] = {0, 0, '\t'},

	[KEY_Q] = {KEYBOARD_FLAG_SHIFT | KEYBOARD_FLAG_CAPSLOCK, 'Q', 'q'},
	[KEY_W] = {KEYBOARD_FLAG_SHIFT | KEYBOARD_FLAG_CAPSLOCK, 'W', 'w'},
	[KEY_E] = {KEYBOARD_FLAG_SHIFT | KEYBOARD_FLAG_CAPSLOCK, 'E', 'e'},
	[KEY_R] = {KEYBOARD_FLAG_SHIFT | KEYBOARD_FLAG_CAPSLOCK, 'R', 'r'},
	[KEY_T] = {KEYBOARD_FLAG_SHIFT | KEYBOARD_FLAG_CAPSLOCK, 'T', 't'},
	[KEY_Y] = {KEYBOARD_FLAG_SHIFT | KEYBOARD_FLAG_CAPSLOCK, 'Y', 'y'},
	[KEY_U] = {KEYBOARD_FLAG_SHIFT | KEYBOARD_FLAG_CAPSLOCK, 'U', 'u'},
	[KEY_I] = {KEYBOARD_FLAG_SHIFT | KEYBOARD_FLAG_CAPSLOCK, 'I', 'i'},
	[KEY_O] = {KEYBOARD_FLAG_SHIFT | KEYBOARD_FLAG_CAPSLOCK, 'O', 'o'},
	[KEY_P] = {KEYBOARD_FLAG_SHIFT | KEYBOARD_FLAG_CAPSLOCK, 'P', 'p'},
		
	[KEY_LEFTBRACE] = {KEYBOARD_FLAG_SHIFT, '{', '['},
	[KEY_RIGHTBRACE] = {KEYBOARD_FLAG_SHIFT, '}', ']'},
	[KEY_ENTER] = {0, 0, '\n'},

	[KEY_A] = {KEYBOARD_FLAG_SHIFT | KEYBOARD_FLAG_CAPSLOCK, 'A', 'a'},
	[KEY_S] = {KEYBOARD_FLAG_SHIFT | KEYBOARD_FLAG_CAPSLOCK, 'S', 's'},
	[KEY_D] = {KEYBOARD_FLAG_SHIFT | KEYBOARD_FLAG_CAPSLOCK, 'D', 'd'},
	[KEY_F] = {KEYBOARD_FLAG_SHIFT | KEYBOARD_FLAG_CAPSLOCK, 'F', 'f'},
	[KEY_G] = {KEYBOARD_FLAG_SHIFT | KEYBOARD_FLAG_CAPSLOCK, 'G', 'g'},
	[KEY_H] = {KEYBOARD_FLAG_SHIFT | KEYBOARD_FLAG_CAPSLOCK, 'H', 'h'},
	[KEY_J] = {KEYBOARD_FLAG_SHIFT | KEYBOARD_FLAG_CAPSLOCK, 'J', 'j'},
	[KEY_K] = {KEYBOARD_FLAG_SHIFT | KEYBOARD_FLAG_CAPSLOCK, 'K', 'k'},
	[KEY_L] = {KEYBOARD_FLAG_SHIFT | KEYBOARD_FLAG_CAPSLOCK, 'L', 'l'},

	[KEY_SEMICOLON] = {KEYBOARD_FLAG_SHIFT, ':', ';'},
	[KEY_APOSTROPHE] = {KEYBOARD_FLAG_SHIFT, '"', '\''},
	[KEY_GRAVE] = {KEYBOARD_FLAG_SHIFT, '~', '`'},
	[KEY_BACKSLASH] = {KEYBOARD_FLAG_SHIFT, '|', '\\'},

	[KEY_Z] = {KEYBOARD_FLAG_SHIFT | KEYBOARD_FLAG_CAPSLOCK, 'Z', 'z'},
	[KEY_X] = {KEYBOARD_FLAG_SHIFT | KEYBOARD_FLAG_CAPSLOCK, 'X', 'x'},
	[KEY_C] = {KEYBOARD_FLAG_SHIFT | KEYBOARD_FLAG_CAPSLOCK, 'C', 'c'},
	[KEY_V] = {KEYBOARD_FLAG_SHIFT | KEYBOARD_FLAG_CAPSLOCK, 'V', 'v'},
	[KEY_B] = {KEYBOARD_FLAG_SHIFT | KEYBOARD_FLAG_CAPSLOCK, 'B', 'b'},
	[KEY_N] = {KEYBOARD_FLAG_SHIFT | KEYBOARD_FLAG_CAPSLOCK, 'N', 'n'},
	[KEY_M] = {KEYBOARD_FLAG_SHIFT | KEYBOARD_FLAG_CAPSLOCK, 'M', 'm'},

	[KEY_COMMA] = {KEYBOARD_FLAG_SHIFT | KEYBOARD_FLAG_CAPSLOCK, '<', ','},
	[KEY_DOT] = {KEYBOARD_FLAG_SHIFT | KEYBOARD_FLAG_CAPSLOCK, '>', '.'},
	[KEY_SLASH] = {KEYBOARD_FLAG_SHIFT | KEYBOARD_FLAG_CAPSLOCK, '?', '/'},
	[KEY_KPASTERISK] = {0, 0, '*'},
	[KEY_SPACE] = {0, 0, ' '},

	[KEY_KP7] = {0, 0, '7'},
	[KEY_KP8] = {0, 0, '8'},
	[KEY_KP9] = {0, 0, '9'},

	[KEY_KPMINUS] = {0, 0, '-'},

	[KEY_KP4] = {0, 0, '4'},
	[KEY_KP5] = {0, 0, '5'},
	[KEY_KP6] = {0, 0, '6'},

	[KEY_KPPLUS] = {0, 0, '+'},

	[KEY_KP1] = {0, 0, '1'},
	[KEY_KP2] = {0, 0, '2'},
	[KEY_KP3] = {0, 0, '3'},
	[KEY_KP0] = {0, 0, '0'},

	[KEY_KPDOT] = {0, 0, '.'},
};

static bool shift;
static bool capslock;

void stdin_hook (uint8_t code)
{
	struct ascii_field *plain;
	uint8_t character;
    bool shifton, capson;

    switch (code)
    {
        case KEY_LEFTSHIFT:
            shift = true;
            return ;
        
        case KEY_LEFTSHIFT + KEY_RELEASE_OFFSET:
            shift = false;
            return ;
        
        case KEY_CAPSLOCK:
            capslock = ~capslock;
            return ;

        default:
    }

	plain = &printable[code];
	character = plain->lower;

    shifton = shift && (plain->flags & KEYBOARD_FLAG_SHIFT);
    capson = capslock && (plain->flags & KEYBOARD_FLAG_CAPSLOCK);

	if (plain->upper)
    {
        if (shift && capson) ;
        else if (shift || capson)
            character = plain->upper;
    }
	if (!character)
		return ;
    printk ("%c", character);
}

ssize_t stdin_open (void)
{
    shift = false;
    capslock = false;
	if (keyboard_insert_hook (stdin_hook) < 0)
        assert (false);

    return 1;
}

ssize_t stdin_close (void)
{
    return 0;
}

ssize_t stdin_read (char *buf, size_t size)
{
}

ssize_t stdin_write (const char *buf, size_t size)
{
    return 0;
}