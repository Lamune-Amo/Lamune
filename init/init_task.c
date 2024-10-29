#include "kernel/task.h"

static struct files_struct init_files = {
    .fd = { 0, },
	.open_fds = 0
};

static struct signal_struct init_signals = {
    .sighandler = NULL
};

struct task_struct init_task = {
    .pid = 0,
    .fs = &init_files,
    .sig_handler = &init_signals
};

