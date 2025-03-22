#include "../../include/libft.h"

/**
 * @brief Deletes and frees an entire linked list.
 *
 * Iterates through the list, deleting each node using `ft_lstdelone`
 * and the provided `del` function. The pointer to the list is set to NULL
 * after clearing to prevent dangling references.
 *
 * @param lst Pointer to the list to clear.
 * @param del Function used to delete the content of each node.
 */
void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*temp;

	if (!lst || !del)
		return;
	while (*lst)
	{
		temp = (*lst)->next;
		ft_lstdelone(*lst, del);
		*lst = temp;
	}
	*lst = NULL; // Ensure the list pointer is reset to NULL
}
