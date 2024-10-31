#include "kernel/task.h"

struct task_struct *current_task = &init_task;

/* scheduling */
size_t schedule (struct task_struct *p)
{
}