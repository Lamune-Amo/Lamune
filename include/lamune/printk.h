#ifndef _LAMUNE_PRINTK_H_
#define _LAMUNE_PRINTK_H_

#include "lamune/types.h"
#include "lamune/stdarg.h"

#define PRINTK_BUFFER_SIZE 256

#define PRINTK_FLAG_WIDTH 0x01

struct printk_info
{
	const char *format;
	char buffer[PRINTK_BUFFER_SIZE + 1];
	uint32_t size;
	uint32_t offset;

	/* flags */
	uint32_t flags;
	struct
	{
		int width;
		char fill;
	} flag;
};

/* extern */
int printk (const char *format, ...);
int vprintk (const char *format, va_list args);

#endif
