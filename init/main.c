#include "arch/VGA.h"
#include "arch/interrupt.h"

void kernel_init (void)
{
	vga_open ();
	interrupt_init ();

	while (1);
}
