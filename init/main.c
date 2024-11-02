#include "arch/keyboard.h"
#include "arch/interrupt.h"
#include "drivers/stdin.h"
#include "drivers/stdout.h"
#include "kernel/task.h"
#include "lamune/unistd.h"
#include "lamune/printk.h"

#include "kernel/schedule.h"
#include "arch/VGA.h"
#include "arch/timer.h"
void timer_screen (void)
{
	unsigned short *video = (unsigned short *) 4254;
	unsigned char color = (WHITE & 0xF) << 4 | (BLACK & 0xF);
	unsigned char ch = 'A';
	volatile int i;

	while (1)
	{
		*video = (color << 8) | (ch & 0xFF);;
		ch++;
		for (i = 0; i < 999999; i++);
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

char stack[64];

struct task_struct timer_task = {
    .state = READY,
    .pid = 2,
    .regs = {
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, ((int)stack + 60), 0
	},
	.pc = (int) timer_screen,
    .remains = 1,
    .fs = &init_files,
    .sig_handler = &init_signals
};
void hexapawn (void);
void kernel_init (void)
{
	char buffer[64];
	int size;

	schedule_init ();
	stdout_ops.open (NULL, NULL);
	keyboard_init ();
	stdin_ops.open (NULL, NULL);
	interrupt_init ();

	printk ("Input> ");

	timer_task.regs[28] = (int)stack + 60;
	timer_task.pc = (int) timer_screen;
	timer_task.remains = 1;
	schedule_register (&timer_task);

	hexapawn ();

	while (1)
	{
		size = read (0, buffer, 64);
		buffer[size] = 0;
		printk ("[%s]\n", buffer);
	}
}
