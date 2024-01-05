#ifndef __RT_SERVICE_H
#define __RT_SERVICE_H

#include "rtthread.h"

rt_inline void rt_list_init(rt_list_t *l)
{
	l->next = l->prev = l;
}

/* 在双向链表头部插入一个节点 */
/*  在双向链表表头后面插入一个节点 */
rt_inline void rt_list_insert_after(rt_list_t *l, rt_list_t *n)
{
	l->next->prev = n;
	n->next = l->next;
	
	l->next = n;
	n->prev = l;
}

/* 在双向链表表头前面插入一个节点 */

rt_inline void rt_list_insert_before(rt_list_t *l, rt_list_t *n)
{
	l->prev->next = n;
	n->prev = l->prev;
	
	
	l->prev = n;
	n->next = l;
}

rt_inline void rt_list_remove(rt_list_t *n)
{
	n->next->prev = n->prev;
	n->prev->next = n->next;
	
	n->next = n->prev = n;
}

rt_inline int rt_list_isempty	(const rt_list_t* l)
{
	return l->next == l;
}	
#endif  /* __RT_SERVICE_H */