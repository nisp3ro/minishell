#include "../../include/libft.h"

/**
 * @brief Adds a node at the beginning of a linked list.
 *
 * This function adds the node pointed to by `new` at the front of the list.
 * The head pointer `lst` is updated to point to the new node.
 *
 * @param lst A pointer to the pointer of the first node of the list.
 * @param new The node to add to the front of the list.
 */
void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (lst && new)
	{
		new->next = *lst;
		*lst = new;
	}
}
