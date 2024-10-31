#include "arch/keyboard.h"
#include "arch/interrupt.h"
#include "drivers/stdin.h"
#include "drivers/stdout.h"
#include "lamune/unistd.h"
#include "lamune/printk.h"

void kernel_init (void)
{
	char buffer[64];
	int size;

	stdout_ops.open (NULL, NULL);
	keyboard_init ();
	stdin_ops.open (NULL, NULL);
	interrupt_init ();

	printk ("Input> ");

	while (1)
	{
		size = read (0, buffer, 64);
		buffer[size] = 0;
		printk ("[%s]\n", buffer);
	}
}
