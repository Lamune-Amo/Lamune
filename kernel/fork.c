#include "mm/allocator.h"
#include "kernel/task.h"
#include "kernel/schedule.h"
#include "lamune/string.h"

pid_t fork (void) // this should be fixed!
{
	struct task_struct *_task;
	struct files_struct *_fs;
	struct signal_struct *_sig;
	char *_stack;

	_task = kmalloc (sizeof (struct task_struct));
	if (!_task)
		goto fail;
	_stack = kmalloc (TASK_STACK_SIZE);
	if (!_stack)
		goto fail;
	_fs = kmalloc (sizeof (struct files_struct));
	if (!_fs)
		goto fail;
	_sig = kmalloc (sizeof (struct signal_struct));
	if (!_sig)
		goto fail;

	/* copy */
	memcpy (_task, CURRENT_TASK, sizeof (struct task_struct));
	memcpy (_stack, CURRENT_TASK->stack, TASK_STACK_SIZE);
	memcpy (_fs, CURRENT_TASK->fs, sizeof (struct files_struct));
	memcpy (_sig, CURRENT_TASK->sig, sizeof (struct signal_struct));

	/* task */
	_task->state = READY;
	_task->pid = task_get_next_pid ();
	_task->remains = 1;
	_task->stack = _stack;
	_task->fs = _fs;
	_task->sig = _sig;

	if (_fork_copy (_task->regs, (uint32_t) CURRENT_TASK->stack, (uint32_t) _stack))
	{
		/* child */
		return 0;
	}

	/* parent */
	/* register */
	schedule_register (_task, true);

	return _task->pid;

fail:
	if (_task)
		kfree (_task);
	if (_stack)
		kfree (_stack);
	if (_fs)
		kfree (_fs);
	return -1;
}

pid_t forkf (const char *name, uint32_t pc)
{
	struct task_struct *_task;
	struct files_struct *_fs;
	struct signal_struct *_sig;
	char *_stack;

	_task = kmalloc (sizeof (struct task_struct));
	if (!_task)
		goto fail;
	_stack = kmalloc (TASK_STACK_SIZE);
	if (!_stack)
		goto fail;
	_fs = kmalloc (sizeof (struct files_struct));
	if (!_fs)
		goto fail;
	_sig = kmalloc (sizeof (struct signal_struct));
	if (!_sig)
		goto fail;

	/* copy */
	memset (_task, 0, sizeof (struct task_struct));
	memset (_stack, 0, TASK_STACK_SIZE);
	memcpy (_fs, CURRENT_TASK->fs, sizeof (struct files_struct));
	memset (_sig, 0, sizeof (struct signal_struct));

	/* task */
	_task->state = READY;
	_task->pid = task_get_next_pid ();
	strcpy (_task->name, name);
	_task->remains = 1;
	_task->stack = _stack;
	_task->fs = _fs;
	_task->sig = _sig;

	_task->regs[28] = (uint32_t) _stack + TASK_STACK_SIZE;
	_task->regs[29] = (uint32_t) task_destructor;
	_task->pc = pc;
	
	CURRENT_TASK->marker.status = MARKER_NONE;
	CURRENT_TASK->marker.task = _task;
	CURRENT_TASK->marker.next = NULL;
	_task->marked = &CURRENT_TASK->marker;

	/* register */
	schedule_register (_task, true);

	return _task->pid;

fail:
	if (_task)
		kfree (_task);
	if (_stack)
		kfree (_stack);
	if (_fs)
		kfree (_fs);
	return -1;
}
