#include "../../include/libft.h"

/**
 * @brief Concatenates two strings into a newly allocated string.
 *
 * This function allocates and returns a new string, which is the result of the concatenation
 * of s1 and s2. The new string is null-terminated. If memory allocation fails, NULL is returned.
 *
 * @param s1 The first null-terminated string.
 * @param s2 The second null-terminated string.
 * @return char* Pointer to the newly allocated concatenated string, or NULL if allocation fails.
 */
char	*ft_strjoin(const char *s1, const char *s2)
{
	size_t	len1;
	size_t	len2;
	char	*res;

	if (!s1 || !s2)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	res = (char *)malloc(sizeof(char) * (len1 + len2 + 1));
	if (!res)
		return (NULL);
	ft_memcpy(res, s1, len1);
	ft_memcpy(res + len1, s2, len2);
	res[len1 + len2] = '\0';
	return (res);
}
