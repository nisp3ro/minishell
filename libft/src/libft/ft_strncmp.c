#include "../../include/libft.h"

/**
 * @brief Compares two strings up to a specified number of characters.
 *
 * This function compares the two null-terminated strings str1 and str2, up to n
 * characters. It returns an integer less than, equal to, or greater than zero if
 * str1 is found, respectively, to be less than, to match, or be greater than str2.
 *
 * @param str1 The first string to compare.
 * @param str2 The second string to compare.
 * @param n    The maximum number of characters to compare.
 * @return int The difference between the first two differing characters (interpreted
 *             as unsigned char), or 0 if the strings are equal up to n characters.
 *
 * @note The standard behavior is undefined if either str1 or str2 is NULL.
 */
int	ft_strncmp(const char *str1, const char *str2, size_t n)
{
	size_t	i;

	i = 0;
	if (n == 0)
		return (0);
	while (i < n - 1 && str1[i] && str2[i] && str1[i] == str2[i])
		i++;
	return ((unsigned char)str1[i] - (unsigned char)str2[i]);
}
