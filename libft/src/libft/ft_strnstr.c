#include "../../include/libft.h"

/**
 * @brief Locates a substring in a string within a limited number of characters.
 *
 * This function searches for the first occurrence of the null-terminated substring s2
 * within the string s1, but searches only the first n characters of s1.
 * If s2 is an empty string, s1 is returned.
 *
 * @param s1 The string to be searched (haystack).
 * @param s2 The substring to search for (needle).
 * @param n The maximum number of characters to search within s1.
 * @return char* A pointer to the beginning of the located substring within s1,
 *               or NULL if the substring is not found within the first n characters.
 */
char	*ft_strnstr(const char *s1, const char *s2, size_t n)
{
	size_t	i;
	size_t	j;

	/* If s2 is empty, return s1 immediately */
	if (s2[0] == '\0')
		return ((char *)s1);
	
	i = 0;
	while (s1[i] && i < n)
	{
		j = 0;
		while (s1[i + j] && s2[j] && (i + j) < n && s1[i + j] == s2[j])
			j++;
		if (s2[j] == '\0')
			return ((char *)(s1 + i));
		i++;
	}
	return (NULL);
}
