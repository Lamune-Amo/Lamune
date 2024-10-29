#include "arch/VGA.h"
#include "arch/keyboard.h"
#include "drivers/stdin.h"
#include "drivers/keycode.h"
#include "lamune/printk.h"
#include "lamune/types.h"
#include "lamune/assert.h"

# define KEYBOARD_FLAG_SHIFT 0x1
# define KEYBOARD_FLAG_CAPSLOCK 0x2

struct ascii_field
{
	uint32_t flags:8;
	uint32_t upper:8;
	uint32_t lower:8;
};

static struct ascii_field printable[] = {
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

	[KEY_COMMA] = {KEYBOARD_FLAG_SHIFT, '<', ','},
	[KEY_DOT] = {KEYBOARD_FLAG_SHIFT, '>', '.'},
	[KEY_SLASH] = {KEYBOARD_FLAG_SHIFT, '?', '/'},
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

char *buffer;
size_t index;
bool stdin_leftshift;
bool stdin_rightshift;
bool stdin_capslock;

void stdin_hook (uint8_t code)
{
	struct ascii_field *plain;
	uint8_t character;
    bool shifton, capson;

    switch (code)
    {
        case KEY_LEFTSHIFT:
            stdin_leftshift = true;
            return ;
        
        case KEY_LEFTSHIFT + KEY_RELEASE_OFFSET:
            stdin_leftshift = false;
            return ;

		case KEY_RIGHTSHIFT:
			stdin_rightshift = true;
            return ;

		case KEY_RIGHTSHIFT + KEY_RELEASE_OFFSET:
			stdin_rightshift = false;
            return ;

        case KEY_CAPSLOCK:
            stdin_capslock = ~stdin_capslock;
            return ;

        default:
			break;
    }

	plain = &printable[code];
	character = plain->lower;
	if (!character)
		return ;

    shifton = (plain->flags & KEYBOARD_FLAG_SHIFT) && (stdin_leftshift || stdin_rightshift);
    capson = (plain->flags & KEYBOARD_FLAG_CAPSLOCK) && stdin_capslock;
	if (plain->upper)
    {
        if (shifton && capson) ;
        else if (shifton || capson)
            character = plain->upper;
    }

	printk ("%c", character);
}

ssize_t stdin_open (void)
{
	buffer = NULL;
    stdin_leftshift = false;
    stdin_rightshift = false;
    stdin_capslock = false;
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
	int old_index;

	index = 0;
	old_index = 0;
	buffer = buf;
	while (1)
	{
		if (old_index != index)
		{
			vga_write (&buffer[index], 1);
			if (buffer[index] == 0x8)
			{
				index--;
				continue;
			}
			if (buffer[index] == '\n')
				break;
			old_index = index;
		}
	}

	buffer = NULL;

	return index > size ? size : index;
}

ssize_t stdin_write (const char *buf, size_t size)
{
    return 0;
}
