#include "kernel/task.h"

pid_t waitpid (pid_t pid)
{
	while (CURRENT_TASK->marker.status != MARKER_DONE);

	CURRENT_TASK->marker.status = MARKER_NONE;
	CURRENT_TASK->marker.next = NULL;

	return pid;
}
