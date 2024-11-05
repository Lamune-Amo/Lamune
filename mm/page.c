#include "mm/page.h"
#include "mm/zone.h"
#include "lamune/string.h"

/* frames */
static struct page mm_frames[PHYSICAL_FRAME_COUNT];

static void page_frame_init (void)
{
	int mapped;
	int i;

	/* used in kernel mapping */
	mapped = ((uint32_t) &_kernel_end >> PAGE_SHIFT) + 1;
	for (i = 0; i < mapped; i++)
	{
		mm_frames[i].flags = PG_reserved;
		mm_frames[i]._refcount = 1;
		LIST_HEAD_INIT (mm_frames[i].chain);
	}
	/* clear unused frames state */
	for (; i < PHYSICAL_FRAME_COUNT; i++)
		memset (&mm_frames[i], 0, sizeof (struct page));
}

uint32_t get_frame_index (const struct page *page)
{
	return (uint32_t) (page - mm_frames);
}

struct page *get_frame (int a)
{
	return &mm_frames[a];
}

void page_init (void)
{
	page_frame_init ();
}
