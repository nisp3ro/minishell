#include "../../include/libft.h"

/**
 * @brief Compares two memory areas.
 *
 * This function compares the first n bytes of the memory areas pointed to by
 * str1 and str2. It returns an integer less than, equal to, or greater than zero if
 * the first n bytes of str1 are found, respectively, to be less than, to match, or be
 * greater than the first n bytes of str2.
 *
 * @param str1 Pointer to the first memory area.
 * @param str2 Pointer to the second memory area.
 * @param n    Number of bytes to compare.
 * @return int The difference between the first two differing bytes (interpreted as
 *             unsigned char), or 0 if all n bytes are equal.
 *
 * @note If n is 0, the function returns 0.
 */
int	ft_memcmp(const void *str1, const void *str2, size_t n)
{
	size_t				i;
	const unsigned char	*s1;
	const unsigned char	*s2;

	s1 = (const unsigned char *)str1;
	s2 = (const unsigned char *)str2;
	i = 0;
	while (i < n)
	{
		if (s1[i] != s2[i])
			return ((int)s1[i] - (int)s2[i]);
		i++;
	}
	return (0);
}
