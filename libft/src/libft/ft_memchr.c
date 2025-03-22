#include "../../include/libft.h"

/**
 * @brief Searches for a character in a memory block.
 *
 * This function scans the first n bytes of the memory area pointed to by str
 * for the first occurrence of the character c (converted to an unsigned char).
 *
 * @param str Pointer to the memory block to search.
 * @param c   The character to search for, passed as an int (converted internally).
 * @param n   The number of bytes to search.
 * @return void* A pointer to the first occurrence of the character in the memory area,
 *               or NULL if the character is not found within the first n bytes.
 */
void	*ft_memchr(const void *str, int c, size_t n)
{
	size_t			i;
	unsigned char	*pstr;
	unsigned char	ch;

	pstr = (unsigned char *)str;
	ch = (unsigned char)c;
	i = 0;
	while (i < n)
	{
		if (pstr[i] == ch)
			return ((void *)(pstr + i));
		i++;
	}
	return (NULL);
}
