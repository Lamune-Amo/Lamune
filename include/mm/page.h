#ifndef _CPU_PAGE_H_
#define _CPU_PAGE_H_

#include "lamune/types.h"
#include "lamune/list.h"

#define ARCH_RESERVED_ADDRESS 0x2400

#define PHYSICAL_MEMORY_SIZE (4ULL * 1024 * 1024)
#define PHYSICAL_FRAME_SIZE (4 * 1024)
#define PHYSICAL_FRAME_COUNT (PHYSICAL_MEMORY_SIZE / PHYSICAL_FRAME_SIZE)

#define PAGE_SIZE PHYSICAL_FRAME_SIZE
#define PAGE_SHIFT 12

/* struct page flags */
# define PG_reserved 1 << 0
# define PG_shared 1 << 1

/* frame */
struct page
{
	uint8_t flags;
	uint8_t _refcount;

	/* used to ordered list in buddy */
	LIST_HEAD(chain);
};

/* extern */
extern char _kernel_end;

void page_init (void);

uint32_t get_frame_index (const struct page *page);
struct page *get_frame (int index);

#endif
