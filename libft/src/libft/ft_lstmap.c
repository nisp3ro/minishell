#include "../../include/libft.h"

/**
 * @brief Creates a new list by applying a function to each node’s content.
 *
 * Iterates through the list `lst`, applies function `f` to each node’s content,
 * and creates a new list with the transformed content. If allocation fails, the
 * function clears and frees the new list using `del`.
 *
 * @param lst The original list.
 * @param f Function to apply to each node's content.
 * @param del Function to free content in case of failure.
 * @return t_list* New list with modified content, or NULL if allocation fails.
 */
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*newlist;
	t_list	*newnode;

	if (!lst || !f || !del)
		return (NULL);
	newlist = NULL;
	while (lst)
	{
		newnode = ft_lstnew(f(lst->content));
		if (!newnode)
		{
			ft_lstclear(&newlist, del);
			return (NULL);
		}
		ft_lstadd_back(&newlist, newnode);
		lst = lst->next;
	}
	return (newlist);
}
