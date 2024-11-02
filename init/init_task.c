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
    .sighandler = NULL
};

struct task_struct init_task = {
    .state = RUNNING,
    .pid = 1,
    .regs = { 0, },
    .pc = 0,
    .remains = 10,
    .fs = &init_files,
    .sig_handler = &init_signals
};
