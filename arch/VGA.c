#include "arch/VGA.h"
#include "fs/file.h"
#include "lamune/types.h"

#define VIDEO_RAM_ADDRESS 4096
#define VIDEO_TEXT_WIDTH 80
#define VIDEO_TEXT_HEIGHT 30

/* 80 x 30 Text Mode */
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

	for (i = 0; i < (VIDEO_TEXT_HEIGHT - 1) * VIDEO_TEXT_WIDTH; i++)
		video[i] = video[i + VIDEO_TEXT_WIDTH];
	for (i = (VIDEO_TEXT_HEIGHT - 1) * VIDEO_TEXT_WIDTH; i < VIDEO_TEXT_HEIGHT * VIDEO_TEXT_WIDTH; i++)
		video[i] = 0;
}

static void vga_forward (void)
{
	int i;

	x++;
	if (x == VIDEO_TEXT_WIDTH)
	{
		x = 0;
		y++;
		if (y == VIDEO_TEXT_HEIGHT)
		{
			vga_scroll ();
			y = VIDEO_TEXT_HEIGHT - 1;
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
	size_t i, j;

	for (i = 0; i < size; i++)
	{
		switch (buf[i])
		{
			/* backspace */
			case 0x8:
				vga_backward ();
				video[x + y * VIDEO_TEXT_WIDTH] = 0;
				break;
			
			case '\n':
				x = 0;
				y++;
				if (y == VIDEO_TEXT_HEIGHT)
				{
					vga_scroll ();
					y = VIDEO_TEXT_HEIGHT - 1;
				}
				break;

			case '\t':
				for (j = 0; j < 4; j++)
				{
					video[x + y * VIDEO_TEXT_WIDTH] = (color << 8) | (' ' & 0xFF);
					vga_forward ();
				}
				break;

			default:
				if (' ' <= buf[i] && buf[i] <= '~')
				{
					video[x + y * VIDEO_TEXT_WIDTH] = (color << 8) | (buf[i] & 0xFF);
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

