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

unsigned short *video = VIDEO_RAM_ADDRESS;

void sleep (void)
{
	volatile int i;

	for (i = 0; i < 499999; i++);
}

void print (char *str)
{
	int i;

	for (i = 0; i < strlen (str); i++)
	{
		*(video++) = FORMAT (WHITE, LIGHT_CYAN, str[i]);
	}
}

void kernel_init (void)
{
	print ("test message. Hello WORLD!!!");
	while (1)
	{
		*video = FORMAT (BLACK, WHITE, ' ');
		sleep();
		*video = FORMAT (BLACK, BLACK, ' ');
		sleep();
	}
}
