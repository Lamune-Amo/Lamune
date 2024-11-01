#include "arch/VGA.h"
#include "lamune/printk.h"
#include "lamune/string.h"
#include "lamune/assert.h"

void assert_panic (const char *msg, const char *file, int line)
{
	vga_clear ();
	printk ("%s from %s:%c", msg, file, line);
}
