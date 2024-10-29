#ifndef _LAMUNE_TASK_TASK_
#define _LAMUNE_TASK_TASK_

#include "fs/vfs.h"
#include "kernel/signal.h"
#include "lamune/types.h"

struct task_struct
{
    ssize_t pid;
    struct files_struct *fs;
    struct signal_struct *sig_handler;
};

extern struct task_struct init_task;

#endif