#include "arch/VGA.h"
#include "lamune/printk.h"

void builtin_clear (int argc, char *argv[])
{
	if (argc > 1)
	{
		printk ("sh: clear: too many arguments\n");
		return ;
	}
	
	vga_clear ();
}

void hexapawn (void);
void builtin_hexapawn (int argc, char *argv[])
{
	if (argc > 1)
	{
		printk ("sh: hexapawn: too many arguments\n");
		return ;
	}
	
	hexapawn ();
}

