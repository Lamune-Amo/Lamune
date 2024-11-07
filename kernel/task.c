#include "kernel/task.h"

uint32_t task_pid = 2;
char kernel_stack[TASK_STACK_SIZE];

pid_t task_get_next_pid (void)
{
	return task_pid++;
}
