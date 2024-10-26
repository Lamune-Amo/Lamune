#ifndef _ARCH_KEYBOARD_
#define _ARCH_KEYBOARD_

#define PS2_KEY_EXTENSION	0xE0
#define PS2_KEY_RELEASE		0xF0

typedef struct _keyboard_contorl keyboard_control_t;
struct _keyboard_contorl
{
	struct
	{
		unsigned char codes[8];
		unsigned char index;
	} ps2;

	unsigned char shift;
	unsigned char capslock;
};

void keyboard_irq (void);

size_t keyboard_open (void);
size_t keyboard_read (char *buf, size_t size);
size_t keyboard_write (const char *buf, size_t size);
size_t keyboard_close (void);

#endif
