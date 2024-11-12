#ifndef _KERNEL_MARKER_
#define _KERNEL_MARKER_

struct task_struct;

#define MARKER_NONE		0
#define MARKER_DONE		1

struct marker_struct
{
	int status;
	/* return value */
	void *value;

	struct task_struct *task;
	struct marker_struct *next;
};

#endif
