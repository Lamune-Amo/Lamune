#ifndef _KERNEL_SIGNAL_
#define _KERNEL_SIGNAL_

#include "lamune/types.h"

#define SIGNAL_NONE			0
#define SIGNAL_INTTERUPT	1

#define SIGNAL_LAST			2

struct signal_struct
{
	void (*handler[SIGNAL_LAST]) (void);
};

void signal_init (void);

void (*signal (int signo, void (*func) (void))) (void);

int kill (pid_t pid, int sig);

#endif
