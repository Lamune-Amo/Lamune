#include "arch/VGA.h"
#include "arch/keyboard.h"
#include "arch/interrupt.h"
#include "mm/page.h"
#include "mm/zone.h"
#include "mm/compact.h"
#include "kernel/schedule.h"
#include "drivers/stdin.h"
#include "drivers/stdout.h"
#include "kernel/task.h"
#include "kernel/signal.h"
#include "lamune/sh.h"
#include "lamune/types.h"
#include "lamune/unistd.h"
#include "lamune/printk.h"
#include "lamune/assert.h"

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

	pid = forkf ("sh", (uint32_t) shell);
	if (pid < 0)
		assert (false);

	vga_cursor_handler ();
}
