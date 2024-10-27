#ifndef _ARCH_INTERRUPT_H_
#define _ARCH_INTERRUPT_H_

#include "lamune/types.h"

typedef void (*interrupt_table_t) (void);

typedef struct _interrupt_stack interrupt_stack_t;
struct _interrupt_stack
{
	uint32_t fp;
	uint32_t sp;
	uint32_t lr;
	char stack[1024];
};

void interrupt_init (void);

#endif
