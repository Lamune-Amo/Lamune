#ifndef _FS_FILE_H_
#define _FS_FILE_H_

#include "lamune/types.h"

#define FILE_READ_BUFFER_SIZE 128
#define NR_OPEN 5

#define SET_BIT_CHAR(val, pos) (val |= (1 << pos))
#define CLEAR_BIT_CHAR(val, pos) (val &= (0xfe << pos))
#define IF_BIT_SET(val, pos) (val & (1 << pos))

struct inode
{

};

struct file_operations
{
	ssize_t (*read) (struct file *fp, char *buf, size_t size);
	ssize_t (*write) (struct file *fp, const char *buf, size_t size);

	int (*open) (struct inode *path, struct file *fp);
	ssize_t (*close) (struct file *fp);
};

struct file
{
	int fd;
	struct file_operations *f_ops;
	char read_buf[FILE_READ_BUFFER_SIZE];
};

struct files_struct {
    struct file *fd[NR_OPEN];
	unsigned char open_fds;
};

#endif