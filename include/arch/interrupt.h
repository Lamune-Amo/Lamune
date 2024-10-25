#ifndef _ARCH_INTERRUPT_H_
#define _ARCH_INTERRUPT_H_

typedef void (*interrupt_table_t[2]) (void);

typedef struct _interrupt_contorl interrupt_control_t;
struct _interrupt_contorl
{
	int fp;
	int sp;
	int lr;
	char stack[64];
};

void interrupt_init (void);

#endif
