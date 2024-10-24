#include "arch/VGA.h"
#include "fs/file.h"
#include "lamune/types.h"

#define KEYBOARD_ADDRESS 8900

static int *keyboard;
static char buffer[KEYBOARD_BUFFER_SIZE];
static size_t head, tail;

void keyboard_irq (void)
{
	int keycode;

	keycode = *keyboard;
	if (keycode == -1)
		return ;

	buffer[tail++] = keycode;
	if (tail == KEYBOARD_BUFFER_SIZE)
		tail = 0;
}

size_t keyboard_open (void)
{
	keyboard = (int *) KEYBOARD_ADDRESS;
	memset (buffer, 0, KEYBOARD_BUFFER_SIZE);
	head = 0;
	tail = 0;

	return 1;
}

size_t keyboard_read (char *buf, size_t size)
{
	while (1)
	{
		if (head == tail)
			continue;

		// test this in other system (ex Visual Studio)
		buffer[head++];
		if (head == KEYBOARD_BUFFER_SIZE)
			head = 0;
		;
	}
	return 1;
}

size_t keyboard_write (const char *buf, size_t size)
{
	return 0;
}

size_t keyboard_close (void)
{
	return 0;
}

