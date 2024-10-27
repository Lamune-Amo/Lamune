#include "arch/VGA.h"
#include "arch/keyboard.h"
#include "arch/interrupt.h"
#include "drivers/stdin.h"
#include "lamune/unistd.h"
#include "lamune/printk.h"

void kernel_init (void)
{
	char buffer[32];
	int size;

	vga_open ();
	keyboard_init ();
	stdin_open ();
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
