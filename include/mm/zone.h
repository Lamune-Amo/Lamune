#ifndef _MM_ZONE_H_
#define _MM_ZONE_H_

#include "mm/page.h"
#include "lamune/types.h"

/* zone */
#define ZONE_COUNT 3
#define ZONE_NAME_MAX 8

enum zone_selector
{
	ZONE_ARCH,
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

	unsigned long allocated;
	unsigned long used;

	/* address */
	unsigned long start;
	unsigned long end;

	/* used to allocate a page */
	struct free_area free_area[MAX_ORDER];
};

/* extern */
void mm_zone_init (void);
struct zone *mm_zone_get (enum zone_selector sel);

void mm_zone_add_page (struct zone *_zone, struct page *page);

void mm_zone_info (void);

#endif
