#include "../../include/libft.h"

/**
 * @brief Counts the number of nodes in a linked list.
 *
 * Iterates through the linked list starting at `lst` and returns the total number of nodes.
 *
 * @param lst Pointer to the first node of the linked list.
 * @return int The number of nodes in the list.
 */
int	ft_lstsize(t_list *lst)
{
	int size = 0;

	while (lst)
	{
		lst = lst->next;
		size++;
	}
	return (size);
}
