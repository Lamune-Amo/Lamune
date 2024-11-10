#include "kernel/task.h"

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
