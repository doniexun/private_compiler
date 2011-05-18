#ifndef __LIST_H
#define __LIST_H

#ifndef NULL
#define NULL ((void *)0)
#endif

struct list_head {
	struct list_head *prev;
	struct list_head *next;
};

#define list_empty(list) ((list)->prev == (list))

static inline void list_init(struct list_head *head)
{
	head->prev = head->next = head;
}

static inline void __list_add(struct list_head *list,
			struct list_head *prev, struct list_head *next)
{
	prev->next = list;
	next->prev = list;
	list->prev = prev;
	list->next = next;
}

static inline void list_add(struct list_head *list,
				struct list_head *head)
{
	__list_add(list, head, head->next);
}

static inline void list_add_tail(struct list_head *list,
				struct list_head *head)
{
	__list_add(list, head->prev, head);
}

static inline void __list_del(struct list_head *prev, struct list_head *next)
{
	prev->next = next;
	next->prev = prev;
}

static inline void list_del(struct list_head *list)
{
	__list_del(list->prev, list->next);
	list->prev = NULL;
	list->next = NULL;
}

#define list_entry(p, type, member)\
	((type *)((void *)(p) - (size_t)&(((type *)0)->member)))

/* NOTE: typeof(*p) */
#define list_for_each_entry(head, p, member)\
	for (p = list_entry((head)->next, typeof(*p), member);\
		&p->member != (head);\
		p = list_entry(p->member.next, typeof(*p), member))

#endif	/* list.h */
