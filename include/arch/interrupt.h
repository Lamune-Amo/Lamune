#ifndef _ARCH_INTERRUPT_H_
#define _ARCH_INTERRUPT_H_

#include "lamune/types.h"

typedef void (*interrupt_table_t) (void);

void interrupt_init (void);

#endif
