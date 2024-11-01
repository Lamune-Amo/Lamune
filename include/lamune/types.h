#ifndef _TYPES_H_
#define _TYPES_H_

#define NULL 0

#define true 1
#define false 0

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

typedef unsigned int size_t;
typedef int ssize_t;

typedef uint8_t bool;

#define offsetof(type, member) ((size_t) &((type *) 0)->member)
#define container_of(ptr, type, member) \
		((type *) ((char *) ptr - offsetof(type, member)))

#endif
