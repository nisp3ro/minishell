#include "../../include/libft.h"

/**
 * @brief Adds a node at the end of a linked list.
 *
 * This function appends the node pointed to by `new` at the end of the linked list.
 * If the list is initially empty (i.e. `*lst` is NULL), the new node becomes the first node.
 *
 * @param lst A pointer to the pointer of the first node of the list.
 * @param new The node to add at the end of the list.
 */
void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*last;

	if (!lst)
		return;
	if (*lst == NULL)
		*lst = new;
	else
	{
		last = ft_lstlast(*lst);
		last->next = new;
	}
}
