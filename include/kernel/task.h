#ifndef _LAMUNE_TASK_TASK_
#define _LAMUNE_TASK_TASK_

#include "fs/vfs.h"
#include "kernel/signal.h"
#include "lamune/types.h"

#define TASK_STACK_SIZE 4096
#define TASK_NAME_SIZE 32

enum task_state
{
	READY = 0,
	RUNNING
};

struct task_struct
{
	uint32_t state;
    pid_t pid;

	/* regs must be after 8 bytes */
	uint32_t regs[30];
	uint32_t pc;

	/* info */
	char name[TASK_NAME_SIZE];

	/* time-slice */
	uint32_t remains;

	/* stack */
	char *stack;

	/* handler */
    struct files_struct *fs;
    struct signal_struct *sig;	
};

extern struct task_struct init_task;
extern struct task_struct *current_task;
extern char kernel_stack[];

#define CURRENT_TASK (current_task)

pid_t task_get_next_pid (void);

#endif
