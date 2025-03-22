#include "../../include/libft.h"

/**
 * @brief Deletes a single node from a linked list.
 *
 * Frees the memory of the node’s content using the function `del`,
 * then frees the node itself. The `next` pointer is not affected.
 *
 * @param lst The node to delete.
 * @param del The function used to delete the content of the node.
 */
void	ft_lstdelone(t_list *lst, void (*del)(void *))
{
	if (!lst || !del)
		return;
	del(lst->content);
	free(lst);
}
