#include "mm/zone.h"
#include "mm/allocator.h"
#include "kernel/task.h"
#include "kernel/schedule.h"
#include "lamune/printk.h"

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

struct task_struct *task_find_by_pid (pid_t pid)
{
	struct task_struct *first, *it;

    first = scheduler->get_first ();
	it = first;
	while (1)
	{
		if (it->pid == pid)
			return it;
		it = scheduler->get_next (it);
		if (it == first)
			break;
	}

	return NULL;
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

	if (CURRENT_TASK->state == DIED)
		printk ("Terminated\n");

	CURRENT_TASK->state = DIED;

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
