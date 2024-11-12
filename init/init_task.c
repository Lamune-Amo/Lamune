#include "drivers/stdin.h"
#include "drivers/stdout.h"
#include "kernel/task.h"

static struct file stdin = {
    .fd = 0,
    .f_ops = &stdin_ops,
};

static struct file stdout = {
    .fd = 1,
    .f_ops = &stdout_ops,
};

static struct file stderr = {
    .fd = 2,
    .f_ops = &stdout_ops,
};

static struct files_struct init_files = {
    .fd = { &stdin, &stdout, &stderr },
	.open_fds = 0b00000111
};

static struct signal_struct init_signals = {
    .handler = {
		[SIGNAL_INTTERUPT] = NULL
	}
};

struct task_struct init_task = {
    .state = RUNNING,
    .pid = 1,
    .regs = { 0, },
    .pc = 0,
	.name = "init",
    .remains = 10,
	.stack = kernel_stack,
    .fs = &init_files,
    .sig = &init_signals,
	.marker = {
		.status = MARKER_NONE,
		.value = NULL,
		.task = NULL,
		.next = NULL
	},
	.marked = NULL
};
