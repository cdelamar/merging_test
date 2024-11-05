#include "../includes/minishell.h"

lnk_list *lst_new(void *_ptr)
{
	lnk_list *new;
	new = malloc(sizeof(lnk_list));
	new->ptr = _ptr;
	new->next = NULL;
	return new;
}

lnk_list *lst_last(lnk_list *col)
{
	if (!col)
		return NULL;
	while (col->next)
		col = col->next;
	return col;
}

void lst_add_back(lnk_list **col, lnk_list *new)
{
	if (*col)
		lst_last(*col)->next = new;
	else
		*col = new;
}

void free_lst(lnk_list **col)
{
	lnk_list *curr;
	lnk_list *tmp;

	if (!col || !*col)
		return ;
	curr = *col;
	while (curr) {
		tmp = curr;
		curr = curr->next;
		free(tmp->ptr);
		free(tmp);
	}
}

void *dino_collector(void *ptr, COLLECTOR_ACTION action)
{
	static lnk_list *collector = NULL;

	if (action == ADD) {
		lst_add_back(&collector, lst_new(ptr));
		return ptr;
	}
	free_lst(&collector);
	collector = NULL;
	return NULL;
}

void *dino_alloc(void *ptr)
{
	if (ptr)
		return dino_collector(ptr, ADD);
	return NULL;
}

void dino_free()
{
	dino_collector(NULL, FREE);
}