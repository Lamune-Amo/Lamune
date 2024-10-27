#ifndef _DRIVERS_STDIN_
#define _DRIVERS_STDIN_

#include "lamune/types.h"

ssize_t stdin_open (void);
ssize_t stdin_close (void);
ssize_t stdin_read (char *buf, size_t size);
ssize_t stdin_write (const char *buf, size_t size);

#endif