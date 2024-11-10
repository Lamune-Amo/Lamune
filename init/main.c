#include "arch/keyboard.h"
#include "arch/interrupt.h"
#include "mm/page.h"
#include "mm/zone.h"
#include "mm/compact.h"
#include "drivers/stdin.h"
#include "drivers/stdout.h"
#include "kernel/task.h"
#include "kernel/signal.h"
#include "lamune/unistd.h"
#include "lamune/printk.h"

#include "mm/allocator.h"
#include "lamune/sh.h"
#include "kernel/schedule.h"
#include "arch/VGA.h"
#include "arch/timer.h"
#include "lamune/assert.h"
void timer_screen (void)
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

void kernel_init (void)
{
	pid_t pid;

	page_init ();
	mm_zone_init ();
	kmalloc_compact_init ();
	task_init ();
	schedule_init ();
	stdout_ops.open (NULL, NULL);
	keyboard_init ();
	signal_init ();
	stdin_ops.open (NULL, NULL);
	interrupt_init ();

	pid = forkf ("time", (uint32_t) timer_screen);
	if (pid < 0)
		assert (false);

	shell ();
}
