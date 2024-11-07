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

/* compact allocator */

# define COMPACT_COUNT 7

/* 32, 64, 128, 256, 512, 1024, 2048 */
# define COMPACT_PAGE_COUNT { 1, 1, 1, 1, 1, 1, 1 }
# define COMPACT_BITMAP_MAX_SIZE (1 * PAGE_SIZE / 32 / 32)

struct compact_area
{
	uint16_t size;
	uint32_t bitmap[COMPACT_BITMAP_MAX_SIZE];

	uint16_t available;

	struct
	{
		void *ptr;
		uint16_t count;
	} page;
};

/* zone */

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

	/* for compact */
	struct compact_area compact_area[COMPACT_COUNT];
};

/* extern */
void mm_zone_init (void);
struct zone *mm_zone_get (enum zone_selector sel);

void mm_zone_add_page (struct zone *_zone, struct page *page);

void mm_zone_info (void);

#endif
