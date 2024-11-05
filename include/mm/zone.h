#ifndef _MMZONE_H_
#define _MMZONE_H_

#include "types.h"
#include "paging.h"

/* zone */
#define ZONE_COUNT 2
#define ZONE_NAME_MAX 8

enum zone_selector
{
	ZONE_KERNEL,
	ZONE_NORMAL
};

/* buddy allocator */

/* this sets a size of biggest block to 1 M (default) */
#define MAX_ORDER 10

struct free_area
{
	/* pages will be chained here */
	struct list_head free_list;
	/* block size */
	unsigned long nr_free;
};

struct zone
{
	char name[ZONE_NAME_MAX];

	/* used to allocate a page */
	struct free_area free_area[MAX_ORDER];
};

/* extern */
void mm_zone_init (void);
struct zone *mm_zone_get (enum zone_selector sel);

void add_page_zone (struct zone *_zone, struct page *page);

#endif
