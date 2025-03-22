#include "../../include/libft.h"

/**
 * @brief Creates a new list node.
 *
 * Allocates (with malloc) and returns a new node. The member variable 'content'
 * is initialized with the value of the parameter 'content' and the variable 'next'
 * is set to NULL.
 *
 * @param content The content to assign to the new node.
 * @return t_list* Pointer to the new node, or NULL if allocation fails.
 */
t_list	*ft_lstnew(void *content)
{
	t_list	*newlist;

	newlist = (t_list *)malloc(sizeof(t_list));
	if (!newlist)
		return (NULL);
	newlist->content = content;
	newlist->next = NULL;
	return (newlist);
}
