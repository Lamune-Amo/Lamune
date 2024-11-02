#include "kernel/task.h"
#include "kernel/schedule.h"
#include "lamune/assert.h"

#define TASK_NUM_MAX 5

struct task_struct *rr_tasks[TASK_NUM_MAX] = {
    &init_task, 0,
};
unsigned int rr_index = 1;

/* Round Robin */
void RR_init (void)
{
	rr_tasks[0] = &init_task;
	rr_index = 1;
}

void RR_register(struct task_struct* task)
{
	int i;

	for (i = 0; i < rr_index; i++)
	{
		if (rr_tasks[i] == task)
			return ;
	}

    if (rr_index < TASK_NUM_MAX)
        rr_tasks[rr_index++] = task;
    else
        assert (false);
}

void RR_unregister(struct task_struct* task)
{
    int i;

    for (i = 0; i < rr_index; i++)
    {
        if (rr_tasks[i] == task)
            break;
    }

    for (; i < rr_index && i < TASK_NUM_MAX - 1; i++)
    {
        rr_tasks[i] = rr_tasks[i + 1];
    }
    rr_index--;
}

struct task_struct* RR_get_next(struct task_struct* task)
{
    int i;

    for (i = 0; i < rr_index; i++)
    {
        if (rr_tasks[i] == task)
        {
            break;
        }
    }

    if (i == rr_index - 1)
        return rr_tasks[0];

    return rr_tasks[i + 1];
}

struct scheduler_ops RR_scheduler = {
	.schedule_init = RR_init,
    .schedule_register = RR_register,
    .schedule_unregister = RR_unregister,
    .get_next = RR_get_next
};
