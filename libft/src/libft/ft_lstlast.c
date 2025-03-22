#include "../../include/libft.h"

/**
 * @brief Returns the last node of a linked list.
 *
 * Iterates through the linked list starting at `lst` and returns the last node.
 * If the list is empty (i.e. `lst` is NULL), the function returns NULL.
 *
 * @param lst Pointer to the first node of the linked list.
 * @return t_list* Pointer to the last node in the list, or NULL if the list is empty.
 */
t_list	*ft_lstlast(t_list *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}
