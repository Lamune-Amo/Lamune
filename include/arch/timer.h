#ifndef _ARCH_TIMER_
#define _ARCH_TIMER_

#include "lamune/types.h"

void timer_irq (void);

ssize_t get_seconds (void);

#endif

