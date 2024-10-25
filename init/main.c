#include "arch/VGA.h"
#include "arch/keyboard.h"
#include "arch/interrupt.h"

void kernel_init (void)
{
	interrupt_init ();
	vga_open ();
	keyboard_open ();

	vga_write ("input> ", 7);

	while (1);
}
