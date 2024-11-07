#ifndef _COMPACE_H_
# define _COMPACE_H_

# include "lamune/types.h"

/* extern */
void kmalloc_compact_init (void);

int kmalloc_is_compact (const void *ptr);

void *kmalloc_compact (uint32_t size);
void *kfree_compact (const void *ptr);
size_t ksize_compact (const void *ptr);

#endif
