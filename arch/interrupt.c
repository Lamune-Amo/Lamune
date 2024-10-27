#include "arch/interrupt.h"
#include "arch/VGA.h"
#include "arch/keyboard.h"

interrupt_stack_t interrupt_stack;
interrupt_table_t interrupt_trap[2];
interrupt_table_t interrupt_irq[2];

void interrupt_init (void)
{
	int idt;

	/* Trap */
	interrupt_trap[0] = /* trap:0 */ NULL;
	interrupt_trap[1] = /* trap:1 */ NULL;

	/* IRQ */
	interrupt_irq[0] = /* device:0 */ NULL;
	interrupt_irq[1] = /* device:1 */ &keyboard_irq;

	asm volatile (
		"mov\t\t%0, IDT\n\t"
		"setvt\t%0, $0\n\t"
		: "=r"(idt)
		:
		: "memory"
	);
}
