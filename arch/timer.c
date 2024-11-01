#include "kernel/task.h"
#include "kernel/schedule.h"

void timer_irq (void)
{
    CURRENT_TASK->remains--;
    if (CURRENT_TASK->remains)
        return ;
    
    /* context switching */
    schedule ();
}
