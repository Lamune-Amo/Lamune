#ifndef _KERNEL_SCHEDULE_
#define _KERNEL_SCHEDULE_

#include "kernel/task.h"

struct scheduler_ops
{
	void (*schedule_init) (void);
	void (*schedule_register) (struct task_struct *task);
	void (*schedule_unregister) (struct task_struct *task);
	struct task_struct * (*get_next) (struct task_struct *task);
};

void schedule_init (void);
void schedule (struct task_struct *p);
void schedule_register (struct task_struct *task);
void schedule_unregister (struct task_struct *task);

/* schedulers */
extern struct scheduler_ops RR_scheduler;

#endif
