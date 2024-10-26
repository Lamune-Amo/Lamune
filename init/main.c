#include "arch/VGA.h"
#include "arch/keyboard.h"
#include "arch/interrupt.h"

#include "lamune/printk.h"

void kernel_init (void)
{
	interrupt_init ();
	vga_open ();
	keyboard_open ();

	printk ("Input> ");

	while (1);
}
