#include "kernel/task.h"
#include "fs/vfs.h"

ssize_t read (int fd, char *buf, size_t size)
{
    if (!IF_BIT_SET (CURRENT_TASK->fs->open_fds, fd))
        return -1;
    
    return CURRENT_TASK->fs->fd[fd]->f_ops->read (CURRENT_TASK->fs->fd[fd], buf, size);
}

ssize_t write (int fd, const char *buf, size_t size)
{
    if (!IF_BIT_SET (CURRENT_TASK->fs->open_fds, fd))
        return -1;
    
    return CURRENT_TASK->fs->fd[fd]->f_ops->write (CURRENT_TASK->fs->fd[fd], buf, size);
}

int open (const char *pathname, int oflag)
{
    /* There is no fs that has path yet so... */
    return 0;
}

int close (int fd)
{
    if (!IF_BIT_SET (CURRENT_TASK->fs->open_fds, fd))
        return -1;

    if (fd == 0 || fd == 1 || fd == 2)
        return 1;
    
    CLEAR_BIT_CHAR (CURRENT_TASK->fs->open_fds, fd);
    return CURRENT_TASK->fs->fd[fd]->f_ops->close (CURRENT_TASK->fs->fd[fd]);
}
