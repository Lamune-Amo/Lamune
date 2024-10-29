#ifndef _KERNEL_SIGNAL_
#define _KERNEL_SIGNAL_

struct signal_struct
{
	void (*sighandler) (void);
};

#endif