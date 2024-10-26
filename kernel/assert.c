#include "arch/VGA.h"
#include "lamune/string.h"
#include "lamune/assert.h"

void assert_panic (const char *msg, const char *file, int line)
{
	vga_clear ();
	vga_write (msg, strlen (msg));
}
