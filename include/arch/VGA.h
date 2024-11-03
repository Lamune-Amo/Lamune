#ifndef _ARCH_VGA_H_
#define _ARCH_VGA_H_

#include "lamune/types.h"

#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define BROWN 6
#define WHITE 7
#define GRAY 8
#define LIGHT_BLUE 9
#define LIGHT_GREEN 10
#define LIGHT_CYAN 11
#define LIGHT_RED 12
#define LIGHT_MAGENTA 13
#define YELLOW 14
#define BRIGHT_WHITE 15

ssize_t vga_open (void);
ssize_t vga_read (char *buf, size_t size);
ssize_t vga_write (const char *buf, size_t size);
ssize_t vga_close (void);

/* extension */
void vga_clear (void);
void vga_cursor (int on);
void vga_set_color (char foreground, char background);

#endif
