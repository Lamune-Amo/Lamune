#ifndef _LAMUNE_TASK_TASK_
#define _LAMUNE_TASK_TASK_

#include "fs/vfs.h"
#include "kernel/signal.h"
#include "lamune/types.h"

enum task_state
{
	READY = 0,
	RUNNING
};

struct task_struct
{
	uint32_t state;
    uint32_t pid;

	/* regs must be after 8 bytes */
	uint32_t regs[30];
	uint32_t pc;

	/* time-slice */
	uint32_t remains;

	/* handler */
    struct files_struct *fs;
    struct signal_struct *sig_handler;
};

extern struct task_struct init_task;
extern struct task_struct *current_task;

#define CURRENT_TASK (current_task)

#endif
