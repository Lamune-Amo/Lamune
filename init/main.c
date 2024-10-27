#include "arch/VGA.h"
#include "arch/keyboard.h"
#include "arch/interrupt.h"

#include "lamune/printk.h"

void keyboard_hook (unsigned char code)
{
	printk ("%d ");
}

void kernel_init (void)
{
	vga_open ();
	keyboard_init ();
	if (keyboard_insert_hook (keyboard_hook) < 0)
	{
		printk ("Failed to insert hook");
		return ;
	}
	interrupt_init ();

	printk ("Input> ");

	while (1);
}
