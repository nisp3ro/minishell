#include "../../include/libft.h"

/**
 * @brief Performs a tracked memory operation based on the given action.
 *
 * Depending on the action, this function either allocates memory and registers it,
 * frees the provided memory pointer (if registered), or clears all tracked allocations.
 *
 * @param action The memory operation to perform (MALLOC, FREE, or CLEAR).
 * @param ptr For the FREE action, the pointer to free; ignored for MALLOC and CLEAR.
 * @param size For the MALLOC action, the number of bytes to allocate;
 *  ignored for FREE and CLEAR.
 * @return void* For MALLOC, returns the allocated memory pointer on success,
 * or NULL on failure. For FREE and CLEAR, returns NULL.
 */
void	*ft_tracked_alloc(t_action action, void *ptr, size_t size)
{
	if (action == MALLOC)
		return (ft_trackalloc(size));
	else if (action == FREE)
	{
		ft_trackedfree(ptr);
		return (NULL);
	}
	else if (action == CLEAR)
	{
		ft_clearallocs();
		return (NULL);
	}
	else
	{
		ft_putendl_fd("Warning: Invalid action passed to ft_tracked_alloc.", 2);
		return (NULL);
	}
}
