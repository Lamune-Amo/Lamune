#ifndef _ARCH_TIMER_
#define _ARCH_TIMER_

#include "lamune/types.h"

void timer_irq (void);

uint32_t timer_get_seconds (void);

#endif

