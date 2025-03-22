#include "../../include/libft.h"

/**
 * @brief Iterates through a linked list and applies a function to each node’s content.
 *
 * Traverses the linked list `lst` and applies the function `f` to each node’s content.
 *
 * @param lst The linked list to iterate through.
 * @param f The function to apply to each node's content.
 */
void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	if (!lst || !f)
		return;
	while (lst)
	{
		f(lst->content);
		lst = lst->next;
	}
}
