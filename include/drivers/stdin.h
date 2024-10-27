#ifndef _DRIVERS_STDIN_
#define _DRIVERS_STDIN_

# define KEYBOARD_FLAG_SHIFT 0x1
# define PLAIN_FLAG_CAPSLOCK 0x2

struct plain_field
{
	uint32_t flag:8;
	uint32_t upper:8;
	uint32_t lower:8;
};

#endif
