#ifndef _FS_FILE_H_
#define _FS_FILE_H_

#include "lamune/types.h"

struct file_io
{
	ssize_t (*open) (void);
	ssize_t (*close) (void);
	ssize_t (*read) (char *buf, size_t size);
	ssize_t (*write) (const char *buf, size_t size);
};

#endif
