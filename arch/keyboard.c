#include "arch/VGA.h"
#include "arch/keyboard.h"
#include "arch/atomic.h"
#include "lamune/types.h"

#define KEYBOARD_ADDRESS 8900

const char keymap[] = {
	[0x11] = 'A',
};

keyboard_control_t keyboard_control;

static int *keyboard;
static char *buffer;
static size_t index;

static char keycodes[8];

__attribute__((interrupt))
void keyboard_irq (void)
{
	int keycode;

	keycode = *keyboard;
	keyboard_control.ps2.codes[keyboard_control.ps2.index++] = keycode;
	
	/* key state */
	if (keyboard_control.ps2.index == 1)
	{
		if (keyboard_control.ps2.codes[0] != PS2_KEY_EXTENSION &&
			keyboard_control.ps2.codes[0] != 0xE1 &&
			keyboard_control.ps2.codes[0] != PS2_KEY_RELEASE)
		{
			/* press: 0 */
			keymap[keyboard_control.ps2.codes[0]];
			goto found;
		}
	}
	else if (keyboard_control.ps2.index == 2)
	{
		if (keyboard_control.ps2.codes[0] == PS2_KEY_EXTENSION &&
			keyboard_control.ps2.codes[1] != PS2_KEY_RELEASE)
		{
			/* press (extension): 1 */
		}
		else if (keyboard_control.ps2.codes[0] == PS2_KEY_RELEASE)
		{
			/* release: 1 */
		}
	}
	else if (keyboard_control.ps2.index == 3)
	{
		if (keyboard_control.ps2.codes[0] == PS2_KEY_EXTENSION &&
			keyboard_control.ps2.codes[1] == PS2_KEY_RELEASE)
		{
			/* release (extension): 2 */
		}
	}
	else if (keyboard_control.ps2.index == 8)
	{
		if (keyboard_control.ps2.codes[0] == 0xE1)
		{
			/* ignore */
			keyboard_control.ps2.index = 0;
		}
	}

	return ;

found:
	//press/release
	//0, 1, 2, 3, 4
	return ;
	
	/*
	if (buffer)
		buffer[index++] = *keyboard;
		*/
}

size_t keyboard_open (void)
{
	keyboard = (int *) KEYBOARD_ADDRESS;
	buffer = NULL;
	index = 0;
	keyboard_control.ps2.index = 0;

	return 1;
}

size_t keyboard_read (char *buf, size_t size)
{
	size_t i;

	index = 0;
	while (atomic_cas_addr ((void **) &buffer, NULL, buf));

	while (1)
	{
		if (index == size)
		{
			
		}
	}

	while (atomic_cas_addr ((void **) &buffer, buf, NULL));

	return size;
}

size_t keyboard_write (const char *buf, size_t size)
{
	return 0;
}

size_t keyboard_close (void)
{
	return 0;
}

