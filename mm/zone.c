#include "mm/page.h"
#include "mm/zone.h"
#include "lamune/string.h"
#include "lamune/printk.h"
#include "lamune/list.h"

/* zones are stacked in kernel memory space in order */
static struct zone zone[ZONE_COUNT];

struct zone *mm_zone_get (enum zone_selector sel)
{
	return &zone[sel];
}

enum zone_selector mm_zone_section (void *addr)
{
	int i;

	for (i = 0; i < ZONE_COUNT; i++)
	{
		if (zone[i].start <= (unsigned long) addr && (unsigned long) addr <= zone[i].end)
			return i;
	}
	
	return -1;
}

static struct page *mm_zone_find_pair (struct zone *_zone, struct page *p, int order)
{
	struct page *it;
	int index;
	int align;

	align = 1 << (order + 1);
	/* if a result is 0, a index of pair page is the sum of index of p and align / 2 */
	index = get_frame_index (p);
	if (index % align)
		index -= (align >> 1);
	else
		index += (align >> 1);

	list_for_each_entry (it, &_zone->free_area[order].free_list, chain)
	{
		if (get_frame_index (it) == index)
			return it;
	}
	return NULL;
}

static struct page *get_pair_leader (struct page *p1, struct page *p2)
{
	if (get_frame_index (p1) > get_frame_index (p2))
		return p2;
	return p1;
}

void mm_zone_add_page (struct zone *_zone, struct page *page)
{
	struct page *pair;
	int order;

	order = 0;
	while (1)
	{
		/* if order has reached the end */
		if (order == MAX_ORDER - 1)
			goto insert;

		/* find a pair of current page without caring the order */
		pair = mm_zone_find_pair (_zone, page, order);
		if (pair)
		{
			list_del (&pair->chain);
			_zone->free_area[order].nr_free--;
			page = get_pair_leader (page, pair);
			order++;
			continue;
		}

insert:
		/* insert the page */
		/* in this point, a pointer page indicates leader of pair */
		list_add (&page->chain, &_zone->free_area[order].free_list);
		_zone->free_area[order].nr_free++;
		break;
	}
}

static void mm_zone_reset (void)
{
	char *names[ZONE_COUNT] = {
		"ARCH", "KERNEL", "NORMAL"
	};
	int i, j;

	/* set a each name */
	for (i = 0; i < ZONE_COUNT; i++)
	{
		strcpy (zone[i].name, names[i]);
		zone[i].allocated = 0;
		zone[i].used = 0;

		zone[i].start = 0;
		zone[i].end = 0;

		for (j = 0; j < MAX_ORDER; j++)
		{
			LIST_HEAD_INIT (zone[i].free_area[j].free_list);
			zone[i].free_area[j].nr_free = 0;
		}
	}
}

static void mm_zone_add_arch (struct zone *_zone)
{
	unsigned int start, end;
	unsigned int i;

	/* set address range */
	_zone->start = 0;
	_zone->end = ARCH_RESERVED_ADDRESS - 1;

	/* add page */
	start = 0;
	end = ARCH_RESERVED_ADDRESS >> PAGE_SHIFT;

	/* get the reserved pages for arch zone */
	for (i = start; i < end; i++)
	{
		mm_zone_add_page (_zone, get_frame (i));
		_zone->allocated += PAGE_SIZE;
	}
}


static void mm_zone_add_kernel (struct zone *_zone)
{
	unsigned int start, end;
	unsigned int i;

	/* add page */
	start = ARCH_RESERVED_ADDRESS >> PAGE_SHIFT;
	end = ((uint32_t) &_kernel_end >> PAGE_SHIFT) + 1;

	/* set address range */
	_zone->start = ARCH_RESERVED_ADDRESS;
	_zone->end = (end << PAGE_SHIFT) - 1;

	/* get the reserved pages for arch zone */
	for (i = start; i < end; i++)
	{
		mm_zone_add_page (_zone, get_frame (i));
		_zone->allocated += PAGE_SIZE;
	}
}

static void mm_zone_add_normal (struct zone *_zone)
{
	unsigned int start, end;
	unsigned int i;

	/* add page */
	start = ((uint32_t) &_kernel_end >> PAGE_SHIFT) + 1;
	end = PHYSICAL_FRAME_COUNT;

	/* set address range */
	_zone->start = start << PAGE_SHIFT;
	_zone->end = (end << PAGE_SHIFT) - 1;

	/* get the reserved pages for arch zone */
	for (i = start; i < end; i++)
	{
		mm_zone_add_page (_zone, get_frame (i));
		_zone->allocated += PAGE_SIZE;
	}
}

static void mm_zone_area_init (void)
{
	struct page *page;
	struct zone *_zone;

	_zone = zone;
	mm_zone_add_arch (&_zone[ZONE_ARCH]);
	mm_zone_add_kernel (&_zone[ZONE_KERNEL]);
	mm_zone_add_normal (&_zone[ZONE_NORMAL]);

	_zone[ZONE_ARCH].used = _zone[ZONE_ARCH].allocated;
	_zone[ZONE_KERNEL].used = _zone[ZONE_KERNEL].allocated;
}

void mm_zone_info (void)
{
	struct zone *_zone;
	unsigned int alloc, used;
	unsigned int i;

	_zone = zone;

	printk ("Memory\n");
	printk ("Zone       Allocated   Used        Address\n");
	printk ("ARCH       %4dKB      %4dKB      %x-%x\n",
			_zone[ZONE_ARCH].allocated / 1024, _zone[ZONE_ARCH].used / 1024,
			_zone[ZONE_ARCH].start, _zone[ZONE_ARCH].end);
	printk ("KERNEL     %4dKB      %4dKB      %x-%x\n",
			_zone[ZONE_KERNEL].allocated / 1024, _zone[ZONE_KERNEL].used / 1024,
			_zone[ZONE_KERNEL].start, _zone[ZONE_KERNEL].end);
	printk ("NORMAL     %4dKB      %4dKB      %x-%x\n",
			_zone[ZONE_NORMAL].allocated / 1024, _zone[ZONE_NORMAL].used / 1024,
			_zone[ZONE_NORMAL].start, _zone[ZONE_NORMAL].end);

	alloc = 0;
	used = 0;
	for (i = 0; i < ZONE_COUNT; i++)
	{
		alloc += _zone[i].allocated / 1024;
		used += _zone[i].used / 1024;
	}
	printk ("ALL        %4dKB      %4dKB      %x-%x\n\n", alloc, used,
			_zone[ZONE_ARCH].start, _zone[ZONE_NORMAL].end);
}

void mm_zone_init (void)
{
	mm_zone_reset ();
	mm_zone_area_init ();
}
