#include "arch/keyboard.h"
#include "arch/interrupt.h"
#include "drivers/stdin.h"
#include "drivers/stdout.h"
#include "lamune/unistd.h"
#include "lamune/printk.h"

void kernel_init (void)
{
	char buffer[32];
	int size;

	stdout_ops.open (NULL, NULL);
	keyboard_init ();
	stdin_ops.open (NULL, NULL);
	interrupt_init ();

	printk ("Input> ");

	while (1)
	{
		vga_cursor (1);
		sleep (1);
		vga_cursor (0);
		sleep (1);
	}
}
