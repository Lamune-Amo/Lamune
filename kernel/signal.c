#include "arch/keyboard.h"
#include "drivers/keycode.h"
#include "kernel/task.h"
#include "kernel/signal.h"
#include "lamune/unistd.h"
#include "lamune/printk.h"
#include "lamune/types.h"
#include "lamune/assert.h"

static char handleable[] = {
	[KEY_C] = SIGNAL_INTTERUPT,

	[KEY_DEFINE_LAST] = 0
};
	
bool signal_ctrl;

void signal_default_interrupt (void)
{
	printk ("^C\n");
}

void (*default_handler[SIGNAL_LAST]) (void) = {
	[SIGNAL_INTTERUPT] = signal_default_interrupt
};

void signal_hook (uint8_t code)
{
	char sig;

    switch (code)
    {
        case KEY_LEFTCTRL:
            signal_ctrl = true;
            return ;
        
        case KEY_LEFTCTRL + KEY_RELEASE_OFFSET:
            signal_ctrl = false;
            return ;

        default:
			break;
    }

	if (!signal_ctrl || code >= KEY_DEFINE_LAST)
		return ;

	sig = handleable[code];
	if (sig >= SIGNAL_LAST || !sig || !FOREGROUND_TASK)
		return ;

	if (FOREGROUND_TASK->sig->handler[sig])
		FOREGROUND_TASK->sig->handler[sig] ();
	else
		default_handler[sig] ();
}

void (*signal (int signo, void (*func) (void))) (void)
{
	void (*prev) (void);

	if (signo >= SIGNAL_LAST || !signo)
		return NULL;

	prev = CURRENT_TASK->sig->handler[signo];
	CURRENT_TASK->sig->handler[signo] = func;

	return prev;
}

void signal_init (void)
{
    signal_ctrl = false;
	if (keyboard_insert_hook (signal_hook) < 0)
        assert (false);
}
