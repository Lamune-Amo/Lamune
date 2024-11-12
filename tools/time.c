#include "arch/VGA.h"
#include "arch/timer.h"

void screen_time (void)
{
	unsigned short *video;
	unsigned char color;
	uint32_t time, minutes, seconds;
	int i;

	color = (WHITE & 0xF) << 4 | (BLACK & 0xF);
	while (1)
	{
		video = (unsigned short *) 4246;

		time = timer_get_seconds ();

		minutes = time / 60;
		seconds = time % 60;

		*(video++) = (color << 8) | ('0' + minutes / 10);
		*(video++) = (color << 8) | ('0' + minutes % 10);
		*(video++) = (color << 8) | (':');
		*(video++) = (color << 8) | ('0' + seconds / 10);
		*(video++) = (color << 8) | ('0' + seconds % 10);
	}
}
