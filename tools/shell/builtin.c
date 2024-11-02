#include "lamune/printk.h"

void builtin_help (int argc, char *argv[])
{
	const char *str = \
		"sh, version 1.0.0-amo-lamune\n" \
		"These shell commands are defined internally.  Type `help' to see this list.\n\n" \
		"hexapawn\n";
	
	if (argc > 1)
	{
		printf ("sh: help: too many arguments");
		return ;
	}
	printk ("%s\n", str);
}
