#include "arch/VGA.h"
#include "arch/keyboard.h"
#include "arch/interrupt.h"
#include "drivers/stdin.h"

#include "lamune/printk.h"

void kernel_init (void)
{
	stdin_open ();
	vga_open ();
	keyboard_init ();
	interrupt_init ();

	printk ("Input> ");

	while (1);
}
