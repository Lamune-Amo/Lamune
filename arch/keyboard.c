#include "arch/VGA.h"
#include "arch/keycode.h"
#include "arch/keyboard.h"
#include "arch/atomic.h"
#include "lamune/types.h"
#include "lamune/printk.h"

#define PS2_KEY_EXTENSION0	0xE0
#define PS2_KEY_EXTENSION1	0xE1
#define PS2_KEY_RELEASE		0xF0
#define PS2_KEY_BUFFER_SIZE	8

#define KEYBOARD_ADDRESS	8900
#define KEYBOARD_HOOK_SIZE	2

const unsigned char PS2_KEYMAP[] = {
	[0x76] = KEY_ESC,
	[0x16] = KEY_1,
	[0x1E] = KEY_2,
	[0x26] = KEY_3,
	[0x25] = KEY_4,
	[0x2E] = KEY_5,
	[0x36] = KEY_6,
	[0x3D] = KEY_7,
	[0x3E] = KEY_8,
	[0x46] = KEY_9,
	[0x45] = KEY_0,
	[0x4E] = KEY_MINUS,
	[0x55] = KEY_EQUAL,
	[0x66] = KEY_BACKSPACE,
	[0x0D] = KEY_TAB,

	[0x15] = KEY_Q,
	[0x1D] = KEY_W,
	[0x24] = KEY_E,
	[0x2D] = KEY_R,
	[0x2C] = KEY_T,
	[0x35] = KEY_Y,
	[0x3C] = KEY_U,
	[0x43] = KEY_I,
	[0x44] = KEY_O,
	[0x4D] = KEY_P,
	[0x54] = KEY_LEFTBRACE,
	[0x5B] = KEY_RIGHTBRACE,

	[0x5A] = KEY_ENTER,
	[0x14] = KEY_LEFTCTRL,
	[0x1C] = KEY_A,
	[0x1B] = KEY_S,
	[0x23] = KEY_D,
	[0x2B] = KEY_F,
	[0x34] = KEY_G,
	[0x33] = KEY_H,
	[0x3B] = KEY_J,
	[0x42] = KEY_K,
	[0x4B] = KEY_L,
	[0x4C] = KEY_SEMICOLON,
	[0x52] = KEY_APOSTROPHE,
	[0x0E] = KEY_GRAVE,
	[0x12] = KEY_LEFTSHIFT,
	[0x5D] = KEY_BACKSLASH,

	[0x1A] = KEY_Z,
	[0x22] = KEY_X,
	[0x21] = KEY_C,
	[0x2A] = KEY_V,
	[0x32] = KEY_B,
	[0x31] = KEY_N,
	[0x3A] = KEY_M,
	[0x41] = KEY_COMMA,
	[0x49] = KEY_DOT,
	[0x4A] = KEY_SLASH,
	[0x59] = KEY_RIGHTSHIFT,
	[0x7C] = KEY_KPASTERISK,
	[0x11] = KEY_LEFTALT,

	[0x29] = KEY_SPACE,
	[0x58] = KEY_CAPSLOCK,

	[0x05] = KEY_F1,
	[0x06] = KEY_F2,
	[0x04] = KEY_F3,
	[0x0C] = KEY_F4,
	[0x03] = KEY_F5,
	[0x0B] = KEY_F6,
	[0x83] = KEY_F7,
	[0x0A] = KEY_F8,
	[0x01] = KEY_F9,
	[0x09] = KEY_F10,
	[0x78] = KEY_F11,
	[0x07] = KEY_F12,

	[0x6C] = KEY_KP7,
	[0x75] = KEY_KP8,
	[0x7D] = KEY_KP9,
	[0x7B] = KEY_KPMINUS,
	[0x6B] = KEY_KP4,
	[0x73] = KEY_KP5,
	[0x74] = KEY_KP6,
	[0x79] = KEY_KPPLUS,
	[0x69] = KEY_KP1,
	[0x72] = KEY_KP2,
	[0x7A] = KEY_KP3,
	[0x70] = KEY_KP0,
	[0x71] = KEY_KPDOT,
	[0x77] = KEY_NUMLOCK,
	[0x7E] = KEY_SCROLLLOCK
};

const char PS2_KEYMAP_EXT[] = {
	[0x4A] = KEY_SLASH,
	[0x5A] = KEY_ENTER,
	[0x11] = KEY_RIGHTALT,
	[0x14] = KEY_RIGHTCTRL,
	[0x75] = KEY_UPARROW,
	[0x6B] = KEY_LEFTARROW,
	[0x72] = KEY_DOWNARROW,
	[0x74] = KEY_RIGHTARROW
};

static int *keyboard;
/* buf */
static unsigned char codes[PS2_KEY_BUFFER_SIZE] = { NULL, };
static unsigned char index;
/* hook */
static keyboard_hook_t hooks[KEYBOARD_HOOK_SIZE] = { NULL, };
static unsigned char registered;

void keyboard_irq (void)
{
	unsigned char id;
	int i;

	codes[index++] = *keyboard;
	
	/* key state */
	if (index == 1)
	{
		if (codes[0] != PS2_KEY_EXTENSION0 &&
			codes[0] != PS2_KEY_EXTENSION1 &&
			codes[0] != PS2_KEY_RELEASE)
		{
			/* press: 0 */
			id = PS2_KEYMAP[codes[index - 1]];
			index = 0;
			goto found;
		}
	}
	else if (index == 2)
	{
		if (codes[0] == PS2_KEY_EXTENSION0 &&
			codes[1] != PS2_KEY_RELEASE)
		{
			/* press (extension): 1 */
			id = PS2_KEYMAP_EXT[codes[index - 1]];
			index = 0;
			goto found;
		}
		else if (codes[0] == PS2_KEY_RELEASE)
		{
			/* release: 1 */
			id = PS2_KEYMAP[codes[index - 1]];
			index = 0;
			if (id)
				id += KEY_RELEASE_OFFSET;
			goto found;
		}
	}
	else if (index == 3)
	{
		if (codes[0] == PS2_KEY_EXTENSION0 &&
			codes[1] == PS2_KEY_RELEASE)
		{
			/* release (extension): 2 */
			id = PS2_KEYMAP_EXT[codes[index - 1]];
			index = 0;
			if (id)
				id += KEY_RELEASE_OFFSET;
			goto found;
		}
	}
	else if (index == 8)
	{
		if (codes[0] == PS2_KEY_EXTENSION1)
		{
			/* ignore */
			index = 0;
		}
	}

	return ;
found:
	if (!id)
		return ;
	
	/* call hooks */
	for (i = 0; i < registered; i++)
		hooks[i] (id);
}

ssize_t keyboard_init (void)
{
	keyboard = (int *) KEYBOARD_ADDRESS;
	index = 0;
	registered = 0;

	return 1;
}

ssize_t keyboard_insert_hook (keyboard_hook_t func)
{
	if (registered == KEYBOARD_HOOK_SIZE)
		return -1;
	
	hooks[registered++] = func;
	return registered - 1;
}

ssize_t keyboard_remove_hook (ssize_t idx)
{
	int i;

	if (idx >= registered)
		return -1;

	--registered;
	for (i = idx; i < registered; i++)
		hooks[i] = hooks[i + 1];
	return 1;
}
