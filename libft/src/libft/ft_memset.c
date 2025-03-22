#include "../../include/libft.h"

/**
 * @brief Fills a memory area with a constant byte.
 *
 * This function sets the first n bytes of the block of memory pointed
 * by str to the specified value (converted to an unsigned char).
 *
 * @param str Pointer to the memory area to be filled.
 * @param c   The byte to fill the memory with.
 * @param n   Number of bytes to fill.
 * @return void* Pointer to the memory area str.
 */
void	*ft_memset(void *str, int c, size_t n)
{
	size_t			i;
	unsigned char	*pstr;
	unsigned char	value;

	i = 0;
	pstr = (unsigned char *)str;
	value = (unsigned char)c;
	while (i < n)
	{
		pstr[i] = value;
		i++;
	}
	return (str);
}
