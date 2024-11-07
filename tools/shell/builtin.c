#include "arch/VGA.h"
#include "mm/zone.h"
#include "mm/allocator.h"
#include "kernel/schedule.h"
#include "lamune/printk.h"
#include "lamune/string.h"

void hexapawn (void);

void builtin_ps (int argc, char *argv[])
{
	if (argc > 1)
	{
		printk ("sh: ps: too many arguments\n");
		return ;
	}
	
	schedule_info ();
}

void builtin_mm (int argc, char *argv[])
{
	if (argc > 1)
	{
		printk ("sh: mm: too many arguments\n");
		return ;
	}
	
	mm_zone_info ();
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

void builtin_clear (int argc, char *argv[])
{
	if (argc > 1)
	{
		printk ("sh: clear: too many arguments\n");
		return ;
	}
	
	vga_clear ();
}

void builtin_hexapawn (int argc, char *argv[])
{
	if (argc > 1)
	{
		printk ("sh: hexapawn: too many arguments\n");
		return ;
	}
	
	hexapawn ();
}

