#ifndef _UNISTD_H_
#define _UNISTD_H_

#include "lamune/types.h"

ssize_t read (int fd, char *buf, size_t size);
ssize_t write (int fd, const char *buf, size_t size);
int open (const char *pathname, int oflag);
int close (int fd);

unsigned int sleep (unsigned int seconds);

#endif
