#include "arch/VGA.h"
#include "arch/keyboard.h"
#include "arch/atomic.h"
#include "lamune/types.h"

#define KEYBOARD_ADDRESS 8900

keyboard_control_t keyboard_control;

static int *keyboard;
static char *buffer;
static size_t index;


const char keymap[] = {
	  0, 0,  0,  0,  0,  0,  0,  0,
	  0, 0,  '\n',  0,  0,  0, '`', 0,
	  0, 0 , 0 , 0,  0, 'q','1', 0,
	  0, 0, 'z','s','a','w','2', 0,
	  0,'c','x','d','e','4','3', 0,
	  0,' ','v','f','t','r','5', 0,
	  0,'n','b','h','g','y','6', 0,
	  0, 0, 'm','j','u','7','8', 0,
	  0,',','k','i','o','0','9', 0,
	  0,'.','/','l',';','p','-', 0,
	  0, 0,'\'', 0,'[', '=', 0, 0,
	  0, 0,13, ']', 0, '\\', 0, 0,
	  0, 0, 0, 0, 0, 0, 127, 0,
	  0,'1', 0,'4','7', 0, 0, 0,
	  '0','.','2','5','6','8', 0, 0,
	  0,'+','3','-','*','9', 0, 0,
	  0, 0, 0, 0
};

__attribute__((interrupt))
void keyboard_irq (void)
{
	static unsigned short *video = (unsigned short *) 4096;

	*(video++) = ((WHITE & 0xF) << 12) | ((BLACK & 0xF) << 8) | keymap[*keyboard];
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

