#ifndef _FS_FILE_H_
#define _FS_FILE_H_

#include "lamune/types.h"

struct file_io
{
	size_t (*open) (void);
	size_t (*close) (void);
	size_t (*read) (char *buf, size_t size);
	size_t (*write) (const char *buf, size_t size);
};

#endif
