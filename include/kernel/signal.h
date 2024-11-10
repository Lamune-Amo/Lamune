#ifndef _KERNEL_SIGNAL_
#define _KERNEL_SIGNAL_

#define SIGNAL_INTTERUPT	0

#define SIGNAL_LAST			1

struct signal_struct
{
	void (*handler[SIGNAL_LAST]) (void);
};

void signal_init (void);

#endif
