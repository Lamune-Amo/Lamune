#ifndef _FS_FILE_H_
#define _FS_FILE_H_

#include "lamune/types.h"

#define NR_OPEN 5

struct inode
{

};

struct file_operations
{
	ssize_t (*read) (struct file *fp, char *buf, size_t size);
	ssize_t (*write) (struct file *fp, const char *buf, size_t size);

	int (*open) (struct inode *path, struct file *fp);
	ssize_t (*close) (void);
};

struct file
{
	struct file_operations *f_ops;
};

struct files_struct {
    struct file *fd[NR_OPEN];
	unsigned char open_fds;
};

#endif