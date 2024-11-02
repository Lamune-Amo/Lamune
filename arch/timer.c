#include "kernel/task.h"
#include "kernel/schedule.h"

#define TIMER_ADDRESS	8904

unsigned int *timer = (unsigned int *) TIMER_ADDRESS;

void timer_irq (void)
{
    CURRENT_TASK->remains--;
    if (CURRENT_TASK->remains)
        return ;
    
    /* context switching */
    schedule (CURRENT_TASK);
}

uint32_t timer_get_seconds (void)
{
	return *timer;
}
