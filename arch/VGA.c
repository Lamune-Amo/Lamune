#include "arch/VGA.h"
#include "fs/file.h"
#include "lamune/types.h"

#define VIDEO_RAM_ADDRESS 4096

/* 80 x 25 Text Mode */
static unsigned short *video;
static unsigned char color;
static size_t x, y;

size_t vga_open (void)
{
	video = (unsigned short *) VIDEO_RAM_ADDRESS;
	color = (WHITE & 0xF) << 4 | (BLACK & 0xF);
	x = 0;
	y = 0;

	return 1;
}

size_t vga_read (char *buf, size_t size)
{
	return 0;
}

static void vga_scroll (void)
{
	int i;

	for (i = 0; i < 24 * 80; i++)
		video[i] = video[i + 80];
	for (i = 24 * 80; i < 25 * 80; i++)
		video[i] = 0;
}

static void vga_forward (void)
{
	int i;

	x++;
	if (x == 80)
	{
		x = 0;
		y++;
		if (y == 25)
		{
			vga_scroll ();
			y = 24;
		}
	}
}

static void vga_backward (void)
{
	int i;

	x--;
	if (x == 0 && y != 0)
	{
		x = 79;
		y--;
	}
}

size_t vga_write (const char *buf, size_t size)
{
	int i, j;

	for (i = 0; i < size; i++)
	{
		switch (buf[i])
		{
			/* backspace */
			case 0x8:
				vga_backward ();
				video[x + y * 80] = 0;
				break;
			
			case '\n':
				x = 0;
				y++;
				if (y == 25)
				{
					vga_scroll ();
					y = 24;
				}
				break;

			case '\t':
				for (j = 0; j < 4; j++)
				{
					video[x + y * 80] = (color << 8) | (' ' & 0xFF);
					vga_forward ();
				}
				break;

			default:
				if (' ' <= buf[i] && buf[i] <= '~')
				{
					video[x + y * 80] = (color << 8) | (buf[i] & 0xFF);
					vga_forward ();
				}
				break;
		}
	}

	return size;
}

size_t vga_close (void)
{
	return 0;
}

