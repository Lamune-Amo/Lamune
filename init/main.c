#include "lamune/string.h"

#define VIDEO_RAM_ADDRESS 4096

#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define BROWN 6
#define WHITE 7
#define GRAY 8
#define LIGHT_BLUE 9
#define LIGHT_GREEN 10
#define LIGHT_CYAN 11
#define LIGHT_RED 12
#define LIGHT_MAGENTA 13
#define YELLOW 14
#define BRIGHT_WHITE 15

#define FORMAT(FORE, BACK, C) (((FORE & 0xF) << 12 | (BACK & 0xF) << 8) | (C & 0xFF))

void sleep (void)
{
	volatile int i;

	for (i = 0; i < 1005000; i++);
}

void print (unsigned short **video, char *str)
{
	int i;

	for (i = 0; i < strlen (str); i++)
	{
		*((*video)++) = FORMAT (YELLOW, LIGHT_MAGENTA, str[i]);
	}
}

void kernel_init (void)
{	

const char keymap[] = {
	  0, 0,  0,  0,  0,  0,  0,  0,
	  0, 0,  0,  0,  0,  0, '`', 0,
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

	unsigned short *video;
	int *keyboard;
	int temp;
	
	video = (unsigned short *) VIDEO_RAM_ADDRESS;
	keyboard = (int *) 8896;

	print (&video, "Input> ");

	while (1)
	{
		temp = *keyboard;
		if (temp == -1)
			continue;

		if (keymap[temp] != 0)
			*(video++) = FORMAT (YELLOW, LIGHT_MAGENTA, keymap[temp]);
	}
}
