#include "mm/allocator.h"
#include "kernel/task.h"
#include "kernel/schedule.h"
#include "lamune/string.h"

int fork (void)
{
	struct task_struct *_task;
	_task = kmalloc (sizeof (struct task_struct));
	/*
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
		*/
	
	/* copy */
	//memcpy (_task, CURRENT_TASK, sizeof (struct task_struct));
	//memcpy (_stack, CURRENT_TASK->stack, TASK_STACK_SIZE);
	//memcpy (_fs, CURRENT_TASK->fs, sizeof (struct files_struct));
	//memcpy (_sig, CURRENT_TASK->sig, sizeof (struct signal_struct));

	//_new_sp = _stack + (current->sp - CURRENT_TASK->stack)

	/* task */
	/*
	_task->state = READY;
	_task->pid = task_get_next_pid ();
	_task->remains = 1;
	_task->stack = _stack;
	_task->fs = _fs;
	_task->sig = _sig;
	*/

	

	/* register */
	schedule_register (_task);

	return _task->pid;

child:
	return 0;

	/*
fail:
	if (_task)
		kfree (_task);
	if (_stack)
		kfree (_stack);
	if (_fs)
		kfree (_fs);
	return -1;
	*/
}
