#include "../../include/libft.h"

/**
 * @brief Copies memory areas with overlap support.
 *
 * Copies n bytes from the source memory area (src) to the destination memory area (dest).
 * The memory areas may overlap; this function handles overlapping regions correctly.
 * If the source and destination are the same, or n is 0, the function returns dest immediately.
 *
 * @param dest Pointer to the destination memory area.
 * @param src  Pointer to the source memory area.
 * @param n    Number of bytes to copy.
 * @return void* Pointer to the destination memory area (dest).
 */
void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*pdest;
	unsigned char	*psrc;

	pdest = (unsigned char *)dest;
	psrc = (unsigned char *)src;
	if (pdest == psrc || n == 0)
		return (dest);
	if (psrc < pdest)
	{
		// Copy backwards to handle overlapping regions
		while (n > 0)
		{
			n--;
			pdest[n] = psrc[n];
		}
	}
	else
	{
		// Use ft_memcpy for non-overlapping regions
		ft_memcpy(pdest, psrc, n);
	}
	return (dest);
}
