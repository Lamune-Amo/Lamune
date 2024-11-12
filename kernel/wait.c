#include "kernel/task.h"

pid_t waitpid (pid_t pid)
{
	struct task_struct *task;

	if (CURRENT_TASK->marker.task->pid != pid)
	{
		task = task_find_by_pid (pid);
		if (!task)
			return -1;
		CURRENT_TASK->marker.task = task;
		CURRENT_TASK->marker.next = task->marked;
		task->marked = &CURRENT_TASK->marker;
	}

	while (CURRENT_TASK->marker.status != MARKER_DONE);

	CURRENT_TASK->marker.status = MARKER_NONE;
	CURRENT_TASK->marker.task = NULL;
	CURRENT_TASK->marker.next = NULL;

	return pid;
}
