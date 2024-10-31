#include "arch/interrupt.h"
#include "arch/VGA.h"
#include "arch/keyboard.h"

interrupt_stack_t interrupt_stack;

/* Trap */
interrupt_table_t interrupt_trap[2] = {
	/* trap:0 */ NULL,
	/* trap:1 */ NULL
};

/* IRQ */
interrupt_table_t interrupt_irq[2] = {
	/* device:0 */ NULL,
	/* device:1 */ &keyboard_irq
};

void interrupt_init (void)
{
	int idt;

	asm volatile (
		"mov\t\t%0, IDT\n\t"
		"setvt\t%0, $0\n\t"
		: "=r"(idt)
		:
		: "memory"
	);
}
