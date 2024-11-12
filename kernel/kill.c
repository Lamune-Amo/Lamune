#include "kernel/task.h"
#include "lamune/types.h"

int kill (pid_t pid, int sig)
{
	struct task_struct *task;

	task = task_find_by_pid (pid);
	if (!task)
		return -1;
	task->state = DIED;
	task->pc = (uint32_t) task_destructor;
	return 0;
}
