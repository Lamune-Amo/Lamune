#include "arch/VGA.h"
#include "fs/vfs.h"
#include "drivers/stdout.h"
#include "lamune/types.h"

/* stdout wrapper */
ssize_t stdout_read (struct file *fp, char *buf, size_t size)
{
    return vga_read (buf, size);
}

ssize_t stdout_write (struct file *fp, const char *buf, size_t size)
{
    return vga_write (buf, size);
}

int stdout_open (struct inode *path, struct file *fp)
{
    return vga_open ();
}

ssize_t stdout_close (struct file *fp)
{
    return vga_close ();
}

struct file_operations stdout_ops = {
	.read = stdout_read,
	.write = stdout_write,
	.open = stdout_open,
	.close = stdout_close
};