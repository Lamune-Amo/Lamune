#ifndef _ARCH_KEYBOARD_
#define _ARCH_KEYBOARD_

typedef struct _keyboard_contorl keyboard_control_t;
struct _keyboard_contorl
{
	unsigned char keys[128];
	unsigned char shift;
	unsigned char capslock;
};

void keyboard_irq (void);

size_t keyboard_open (void);
size_t keyboard_read (char *buf, size_t size);
size_t keyboard_write (const char *buf, size_t size);
size_t keyboard_close (void);

#endif
