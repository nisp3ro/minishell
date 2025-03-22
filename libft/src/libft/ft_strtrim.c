#include "../../include/libft.h"

/**
 * @brief Trims specified characters from the beginning and end of a string.
 *
 * Allocates and returns a copy of the string 's1' with all characters found in 'set'
 * removed from the beginning and the end. If 's1' or 'set' is NULL, the function returns
 * NULL. If no characters remain after trimming, an empty string is returned.
 *
 * @param s1 The source string to be trimmed.
 * @param set The set of characters to remove from both ends of s1.
 * @return char* Pointer to the newly allocated trimmed string, or NULL if allocation fails.
 */
char	*ft_strtrim(char const *s1, char const *set)
{
	char	*res;
	int		i;
	int		istart;
	int		iend;

	if (!s1 || !set)
		return (NULL);
	istart = 0;
	iend = ft_strlen(s1);
	while (s1[istart] && ft_strchr(set, s1[istart]))
		istart++;
	while (iend > istart && ft_strchr(set, s1[iend - 1]))
		iend--;
	if (iend <= istart)
		return (ft_calloc(1, 1));
	res = (char *)malloc(sizeof(char) * (iend - istart + 1));
	if (!res)
		return (NULL);
	i = 0;
	while (istart < iend)
		res[i++] = s1[istart++];
	res[i] = '\0';
	return (res);
}
