#ifndef _LAMUNE_LIST_H_
#define _LAMUNE_LIST_H_

struct list_head
{
	struct list_head *next;
	struct list_head *prev;
};

# define LIST_HEAD(name) struct list_head name;
# define LIST_HEAD_INIT(head) (head.next = head.prev = NULL, head)

# define list_for_each(ptr, head) \
	for (ptr = (head)->next; ptr; ptr = ptr->next)

# define list_entry(ptr, type, member) container_of(ptr, type, member)

# define list_for_each_entry(ptr, head, member) \
	for (ptr = list_entry((head)->next, typeof (*ptr), member); &ptr->member; \
		ptr = list_entry(ptr->member.next, typeof (*ptr), member))

void list_add (struct list_head *m, struct list_head *head);
void list_del (struct list_head *entry);

#endif
