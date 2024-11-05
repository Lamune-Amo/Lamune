#include "arch/VGA.h"
#include "mm/zone.h"
#include "lamune/printk.h"

void hexapawn (void);

void builtin_mm (int argc, char *argv[])
{
	if (argc > 1)
	{
		printk ("sh: mm: too many arguments\n");
		return ;
	}
	
	mm_zone_info ();
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

