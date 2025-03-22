#include "../../include/libft.h"

/**
 * @brief Copies a memory area.
 *
 * This function copies n bytes from the memory area pointed to by src to the memory area pointed to by dest.
 * The memory areas must not overlap; use ft_memmove if they do.
 *
 * @note The standard memcpy does not perform any NULL pointer checks. If either dest or src is NULL
 *       (and n > 0), the behavior is undefined. It is the caller's responsibility to ensure that
 *       both pointers are valid.
 *
 * @param dest Pointer to the destination memory area.
 * @param src  Pointer to the source memory area.
 * @param n    Number of bytes to copy.
 * @return void* Pointer to the destination memory area (dest).
 */
void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		((unsigned char *)dest)[i] = ((unsigned char *)src)[i];
		i++;
	}
	return (dest);
}
