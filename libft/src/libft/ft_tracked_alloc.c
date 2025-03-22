#include "../../include/libft.h"

/* Global allocation list pointer */
static t_alloc	*g_alloc_list = NULL;

/**
 * @brief Adds an allocation record to the allocation list.
 *
 * Registers the pointer 'ptr' in the global allocation list.
 *
 * @param ptr Pointer to the allocated memory.
 * @return int Returns 1 on success, 0 if memory allocation for the record fails.
 */
static int	add_alloc(void *ptr)
{
	t_alloc	*new_node;

	new_node = (t_alloc *)malloc(sizeof(t_alloc));
	if (!new_node)
		return (0); // Failed to allocate memory for the tracking record
	new_node->ptr = ptr;
	new_node->next = g_alloc_list;
	g_alloc_list = new_node;
	return (1);
}

/**
 * @brief Removes an allocation record from the allocation list.
 *
 * Searches the global allocation list for the given pointer 'ptr' and
 * removes its record if found.
 *
 * @param ptr Pointer to the allocated memory to remove.
 * @return int Returns 1 if the record was found and removed, 0 otherwise.
 */
static int	remove_alloc(void *ptr)
{
	t_alloc	*prev;
	t_alloc	*curr;

	prev = NULL;
	curr = g_alloc_list;
	while (curr)
	{
		if (curr->ptr == ptr)
		{
			if (prev)
				prev->next = curr->next;
			else
				g_alloc_list = curr->next;
			free(curr); // Free the tracking node
			return (1);
		}
		prev = curr;
		curr = curr->next;
	}
	return (0); // Pointer not found in the allocation list
}

/**
 * @brief Allocates memory and registers the allocation.
 *
 * This function wraps malloc. After allocation, the pointer is registered
 * in the global allocation list. If registration fails, the allocated memory
 * is freed and NULL is returned.
 *
 * @param size Number of bytes to allocate.
 * @return void* Pointer to the allocated memory, or NULL if allocation fails.
 */
void	*ft_trackalloc(size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
		return (NULL);
	if (!add_alloc(ptr))
	{
		free(ptr); // Free allocated memory if tracking fails
		return (NULL);
	}
	return (ptr);
}

/**
 * @brief Frees allocated memory and removes its record.
 *
 * This function frees the memory pointed to by 'ptr' and removes the corresponding
 * allocation record from the global list. If the pointer is not registered, an error
 * message is written to the error channel.
 *
 * @param ptr Pointer to the memory to free.
 */
void	ft_trackedfree(void *ptr)
{
	if (!ptr)
		return ;
	if (remove_alloc(ptr))
		free(ptr);
	else
		ft_putendl_fd("Warning: Attempt to free unregistered memory.", 2);
}

/**
 * @brief Frees all remaining allocated memory.
 *
 * Iterates through the global allocation list and frees each allocated memory block,
 * as well as the allocation record itself. This function is intended to be called at
 * program termination to clean up any leftover allocations.
 */
void	ft_clearallocs(void)
{
	t_alloc	*curr;
	t_alloc	*next;

	curr = g_alloc_list;
	while (curr)
	{
		next = curr->next;
		free(curr->ptr); // Free the allocated memory
		free(curr);      // Free the tracking node
		curr = next;
	}
	g_alloc_list = NULL;
}
