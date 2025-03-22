#include "../../include/libft.h"

/**
 * @brief Allocates memory for an array and initializes it to zero.
 *
 * Allocates memory for an array of num elements, each of size bytes, and initializes
 * all bytes in the allocated memory to zero. This function wraps malloc and ft_memset
 * to provide functionality similar to the standard calloc.
 *
 * @param num The number of elements to allocate.
 * @param size The size in bytes of each element.
 * @return void* Pointer to the allocated and zero-initialized memory, or NULL if
 *               the allocation fails.
 */
void	*ft_calloc(size_t num, size_t size)
{
	void	*spc;

	spc = malloc(num * size);
	if (spc == NULL)
		return (NULL);
	ft_memset(spc, 0, num * size);
	return (spc);
}
