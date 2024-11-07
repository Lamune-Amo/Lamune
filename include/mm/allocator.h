#ifndef _MM_ALLOCATOR_H_
# define _MM_ALLOCATOR_H_

# include "lamune/types.h"

struct zone;

struct page *alloc_pages (struct zone *zone, int num);
void *free_pages (struct zone *zone, struct page *page);

void *kmalloc (size_t size);
void kfree (const void *ptr);
size_t ksize (const void *ptr);

#endif

