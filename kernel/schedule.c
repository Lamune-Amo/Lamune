#include "kernel/task.h"
#include "kernel/schedule.h"

struct task_struct *current_task = &init_task;
struct scheduler_ops *scheduler = &RR_scheduler;

/* scheduling */
void schedule (struct task_struct *task)
{
    /* This function acts as a kind of wrapper that calls the internal scheulder */
    /* !!! This can be called from INTERRUPT handler */
    struct task_struct *next;

    task->state = READY;
	asm volatile (
		"mov\t\t%0, r26\n\t"
		: "=r"(task->pc)
		:
		: "memory"
	);

    next = scheduler->get_next (task);
    next->remains = 1;
    next->state = RUNNING;
    asm volatile (
		"mov\t\tr26, %0\n\t"
		: 
		: "r"(next->pc)
		: "memory"
	);

    CURRENT_TASK = next;

    /* restoring the stack frame is performed by INT handler */
}

void schedule_register (struct task_struct *task)
{
    scheduler->schedule_register (task);
}

void schedule_unregister (struct task_struct *task)
{
    scheduler->schedule_unregister (task);
}
