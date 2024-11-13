#include "arch/VGA.h"

void colorful (void)
{
	char foreground;
	char background;
	int distance;
	volatile int i;

	foreground = BRIGHT_WHITE;
	background = BLACK;
	distance = 1;
	while (1)
	{
		vga_set_color (foreground, background);

		foreground -= distance;
		background += distance;
		if (background > 0xF)
		{
			background = 0x0;
			distance++;
		}
		if (distance > 5)
			distance = 1;

		for (i = 0; i < 999; i++);
	}
}
