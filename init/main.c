#include "arch/keyboard.h"
#include "arch/interrupt.h"
#include "mm/page.h"
#include "mm/zone.h"
#include "drivers/stdin.h"
#include "drivers/stdout.h"
#include "kernel/task.h"
#include "lamune/unistd.h"
#include "lamune/printk.h"

#include "lamune/sh.h"
#include "kernel/schedule.h"
#include "arch/VGA.h"
#include "arch/timer.h"
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

static struct file stdin = {
    .fd = 0,
    .f_ops = &stdin_ops,
};

static struct file stdout = {
    .fd = 1,
    .f_ops = &stdout_ops,
};

static struct file stderr = {
    .fd = 2,
    .f_ops = &stdout_ops,
};

static struct files_struct init_files = {
    .fd = { &stdin, &stdout, &stderr },
	.open_fds = 0b00000111
};

static struct signal_struct init_signals = {
    .sighandler = NULL
};

char stack[128];

struct task_struct timer_task = {
    .state = READY,
    .pid = 2,
    .regs = {
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, ((int)stack + 124), 0
	},
	.pc = (int) timer_screen,
    .remains = 1,
    .fs = &init_files,
    .sig_handler = &init_signals
};

void kernel_init (void)
{
	char buffer[64];
	int size;

	page_init ();
	mm_zone_init ();
	schedule_init ();
	stdout_ops.open (NULL, NULL);
	keyboard_init ();
	stdin_ops.open (NULL, NULL);
	interrupt_init ();

	/* temporal */
	timer_task.regs[28] = (int)stack + 124;
	timer_task.pc = (int) timer_screen;
	timer_task.remains = 1;
	schedule_register (&timer_task);

	/* temporal */
	shell ();
}
