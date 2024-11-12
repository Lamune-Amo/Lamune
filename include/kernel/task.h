#ifndef _LAMUNE_TASK_TASK_
#define _LAMUNE_TASK_TASK_

#include "fs/vfs.h"
#include "kernel/signal.h"
#include "kernel/marker.h"
#include "lamune/types.h"

#define TASK_STACK_SIZE 4096
#define TASK_NAME_SIZE 32

enum task_state
{
	READY = 0,
	RUNNING,
	DIED
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

	/* marker */
	struct marker_struct marker;
	struct marker_struct *marked;
};

extern struct task_struct init_task;
extern struct task_struct *current_task;
extern char kernel_stack[];

extern void task_init (void);
extern int _fork_copy (uint32_t *regs, uint32_t old_stack, uint32_t new_stack);

extern struct task_struct *task_find_by_pid (pid_t pid);

extern void task_destructor (void);

#define CURRENT_TASK (current_task)

pid_t task_get_next_pid (void);

#endif
