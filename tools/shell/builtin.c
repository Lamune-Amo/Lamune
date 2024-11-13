#include "arch/VGA.h"
#include "mm/zone.h"
#include "mm/allocator.h"
#include "kernel/schedule.h"
#include "kernel/wait.h"
#include "kernel/signal.h"
#include "lamune/tools.h"
#include "lamune/types.h"
#include "lamune/unistd.h"
#include "lamune/printk.h"
#include "lamune/string.h"
#include "lamune/assert.h"

void builtin_ps (int argc, char *argv[])
{
	pid_t pid;

	if (argc > 1)
	{
		printk ("sh: ps: too many arguments\n");
		return ;
	}
	
	pid = forkf ("ps", (uint32_t) schedule_info);
	if (pid < 0)
		assert (false);
	waitpid (pid);
}

void builtin_mm (int argc, char *argv[])
{
	pid_t pid;

	if (argc > 1)
	{
		printk ("sh: mm: too many arguments\n");
		return ;
	}
	
	pid = forkf ("mm", (uint32_t) mm_zone_info);
	if (pid < 0)
		assert (false);
	waitpid (pid);
}

void builtin_time (int argc, char *argv[])
{
	pid_t pid;

	if (argc > 1)
	{
		printk ("sh: time: too many arguments\n");
		return ;
	}
	
	pid = forkf ("time", (uint32_t) screen_time);
	if (pid < 0)
		assert (false);
}

void builtin_kill (int argc, char *argv[])
{
	if (argc == 1)
	{
		printk ("sh: kill: too few arguments\n");
		return ;
	}
	if (argc > 2)
	{
		printk ("sh: kill: too many arguments\n");
		return ;
	}
	
	if (kill (atoi (argv[1]), 1) < 0)
		printk ("sh: kill: (%d) - no such process\n", atoi (argv[1]));
}

void builtin_clear (int argc, char *argv[])
{
	if (argc > 1)
	{
		printk ("sh: clear: too many arguments\n");
		return ;
	}
	
	vga_clear ();
}

void builtin_colorful (int argc, char *argv[])
{
	pid_t pid;

	if (argc > 1)
	{
		printk ("sh: colorful: too many arguments\n");
		return ;
	}

	pid = forkf ("cOLorfUl", (uint32_t) colorful);
	if (pid < 0)
		assert (false);
}

void builtin_hexapawn (int argc, char *argv[])
{
	pid_t pid;

	if (argc > 1)
	{
		printk ("sh: hexapawn: too many arguments\n");
		return ;
	}
	
	pid = forkf ("hexapawn", (uint32_t) hexapawn);
	if (pid < 0)
		assert (false);
	waitpid (pid);
}

void builtin_hexapawn_auto (int argc, char *argv[])
{
	pid_t pid;

	if (argc > 1)
	{
		printk ("sh: hexapawn: too many arguments\n");
		return ;
	}
	
	pid = forkf ("hexapawn", (uint32_t) hexapawn_auto);
	if (pid < 0)
		assert (false);
	waitpid (pid);
}

void builtin_malloc (int argc, char *argv[])
{
	char *m;

	if (argc != 2)
	{
		printk ("sh: malloc: too many arguments\n");
		return ;
	}

	m = kmalloc (atoi (argv[1]));
	printk ("malloc request: %d\n", atoi (argv[1]));
	printk ("%d %d\n", m, ksize (m));
}

void builtin_free (int argc, char *argv[])
{
	if (argc != 2)
	{
		printk ("sh: malloc: too many arguments\n");
		return ;
	}

	printk ("free request: %d\n", atoi (argv[1]));
	kfree ((void *) atoi (argv[1]));
}

void builtin_sp (int argc, char *argv[])
{
	if (argc > 1)
	{
		printk ("sh: sp: too many arguments\n");
		return ;
	}
	
	int sp;

	asm volatile (
		"mov\t\t%0, sp\n\t"
		: "=r"(sp)
		:
		: "memory"
	);

	printk ("sp: %x\nstack head: %x\n", sp, kernel_stack);
}
