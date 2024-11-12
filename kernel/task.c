#include "mm/zone.h"
#include "mm/allocator.h"
#include "kernel/task.h"
#include "kernel/schedule.h"

char kernel_stack[TASK_STACK_SIZE];

/* task */
uint32_t task_pid;

pid_t task_get_next_pid (void)
{
	return task_pid++;
}

void task_init (void)
{
	task_pid = 2;
}

void task_destructor (void)
{
	struct marker_struct *marked;

	marked = CURRENT_TASK->marked;
	while (marked)
	{
		marked->status = MARKER_DONE;
		marked->value = NULL;
		marked = marked->next;
	}

	/* lock */
	asm volatile (
		"lock\n\t"
	);

	/* remove myself */
	schedule_unregister (CURRENT_TASK, false);

	if (mm_zone_section (CURRENT_TASK) == ZONE_NORMAL)
	{
		kfree (CURRENT_TASK->sig);
		kfree (CURRENT_TASK->fs);
		kfree (CURRENT_TASK->stack);
		kfree (CURRENT_TASK);
	}

	/* unlock */
	asm volatile (
		"lock\n\t"
	);

	/* halt */
	asm volatile (
		"_task_hlt:\n\t"
		"jmp\t\t_task_hlt\n\t"
		:
		:
		:
	);
}
