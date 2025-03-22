#include "../../include/libft.h"

/**
 * @brief Sets a block of memory to zero.
 *
 * This function writes zeroes to the first n bytes of the memory area
 * pointed to by str.
 *
 * @param str Pointer to the memory area to clear.
 * @param n   Number of bytes to set to zero.
 */
void	ft_bzero(void *str, size_t n)
{
	ft_memset(str, 0, n);
}
