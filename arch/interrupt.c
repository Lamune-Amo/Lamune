#include "arch/VGA.h"

const char keymap[] = {
	  0, 0,  0,  0,  0,  0,  0,  0,
	  0, 0,  '\n',  0,  0,  0, '`', 0,
	  0, 0 , 0 , 0,  0, 'q','1', 0,
	  0, 0, 'z','s','a','w','2', 0,
	  0,'c','x','d','e','4','3', 0,
	  0,' ','v','f','t','r','5', 0,
	  0,'n','b','h','g','y','6', 0,
	  0, 0, 'm','j','u','7','8', 0,
	  0,',','k','i','o','0','9', 0,
	  0,'.','/','l',';','p','-', 0,
	  0, 0,'\'', 0,'[', '=', 0, 0,
	  0, 0,13, ']', 0, '\\', 0, 0,
	  0, 0, 0, 0, 0, 0, 127, 0,
	  0,'1', 0,'4','7', 0, 0, 0,
	  '0','.','2','5','6','8', 0, 0,
	  0,'+','3','-','*','9', 0, 0,
	  0, 0, 0, 0
};

__attribute__((interrupt))
void interrupt (int signal)
{
}

void irq (int signal)
{
	int *keyboard;
	int temp;

	keyboard = (int *) 8900;

	temp = *keyboard;
	if (temp == -1)
		return ;

	if (keymap[temp] != 0)
		vga_write (&keymap[temp], 1);
}

void interrupt_init (void)
{
	int idt;

	asm volatile (
		"mov\t\t%0, IDT\n\t"
		"lidt\t%0\n\t"
		: "=r"(idt)
		:
		: "memory"
	);
}
