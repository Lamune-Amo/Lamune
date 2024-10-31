#include "arch/VGA.h"
#include "lamune/types.h"

#define VIDEO_RAM_ADDRESS 4096
#define VIDEO_TEXT_WIDTH 80
#define VIDEO_TEXT_HEIGHT 30

/* 80 x 30 Text Mode */
static unsigned short *vga_video;
static unsigned char vga_color;
static size_t vga_x, vga_y;

ssize_t vga_open (void)
{
	vga_video = (unsigned short *) VIDEO_RAM_ADDRESS;
	vga_color = (WHITE & 0xF) << 4 | (BLACK & 0xF);
	vga_x = 0;
	vga_y = 0;

	return 1;
}

ssize_t vga_read (char *buf, size_t size)
{
	return 0;
}

static void vga_scroll (void)
{
	int i;

	for (i = 0; i < (VIDEO_TEXT_HEIGHT - 1) * VIDEO_TEXT_WIDTH; i++)
		vga_video[i] = vga_video[i + VIDEO_TEXT_WIDTH];
	for (i = (VIDEO_TEXT_HEIGHT - 1) * VIDEO_TEXT_WIDTH; i < VIDEO_TEXT_HEIGHT * VIDEO_TEXT_WIDTH; i++)
		vga_video[i] = 0;
}

static void vga_forward (void)
{
	int i;

	vga_x++;
	if (vga_x == VIDEO_TEXT_WIDTH)
	{
		vga_x = 0;
		vga_y++;
		if (vga_y == VIDEO_TEXT_HEIGHT)
		{
			vga_scroll ();
			vga_y = VIDEO_TEXT_HEIGHT - 1;
		}
	}
}

static void vga_backward (void)
{
	int i;

	if (vga_x == 0 && vga_y != 0)
	{
		vga_x = VIDEO_TEXT_WIDTH - 1;
		vga_y--;

		return ;
	}
	vga_x--;
}

ssize_t vga_write (const char *buf, size_t size)
{
	size_t i, j;

	for (i = 0; i < size; i++)
	{
		switch (buf[i])
		{
			/* backspace */
			case 0x8:
				vga_backward ();
				vga_video[vga_x + vga_y * VIDEO_TEXT_WIDTH] = 0;
				break;
			
			case '\n':
				vga_x = 0;
				vga_y++;
				if (vga_y == VIDEO_TEXT_HEIGHT)
				{
					vga_scroll ();
					vga_y = VIDEO_TEXT_HEIGHT - 1;
				}
				break;

			case '\t':
				for (j = 0; j < 4; j++)
				{
					vga_video[vga_x + vga_y * VIDEO_TEXT_WIDTH] = (vga_color << 8) | (' ' & 0xFF);
					vga_forward ();
				}
				break;

			default:
				if (' ' <= buf[i] && buf[i] <= '~')
				{
					vga_video[vga_x + vga_y * VIDEO_TEXT_WIDTH] = (vga_color << 8) | (buf[i] & 0xFF);
					vga_forward ();
				}
				break;
		}
	}

	return size;
}

ssize_t vga_close (void)
{
	return 0;
}

/* -------------------------------------------------------------------------------------- */
/* extension																			  */
/* -------------------------------------------------------------------------------------- */
void vga_clear (void)
{
	int i;

	for (i = 0; i < VIDEO_TEXT_HEIGHT * VIDEO_TEXT_WIDTH; i++)
		vga_video[i] = 0;
}

void vga_cursor (int on)
{
	if (on)
	{
		vga_video[vga_x + vga_y * VIDEO_TEXT_WIDTH] = ((WHITE & 0xF) << 12 | (WHITE & 0xF) << 8) | (' ' & 0xFF);
	}
	else
	{
		vga_video[vga_x + vga_y * VIDEO_TEXT_WIDTH] = ((BLACK & 0xF) << 12 | (BLACK & 0xF) << 8) | (' ' & 0xFF);
	}
}
