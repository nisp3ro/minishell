#include "../../include/libft.h"

/**
 * @brief Extracts a substring from a string.
 *
 * Allocates and returns a substring from the string `s`. The substring begins at the
 * index `start` and is of maximum length `len`. If `start` is greater than or equal to
 * the length of `s`, an empty string is returned. If the requested length exceeds the
 * number of characters available from `start` to the end of `s`, the substring will
 * contain only the available characters.
 *
 * @param s The source string.
 * @param start The starting index from which to extract the substring.
 * @param len The maximum length of the substring.
 * @return char* Pointer to the newly allocated substring, or NULL if allocation fails
 *               or if `s` is NULL.
 */
char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*subs;
	size_t	slen;

	if (!s)
		return (NULL);
	slen = ft_strlen(s);
	if (start >= slen)
		return (ft_strdup("")); /* Return an empty string if start is beyond s */
	if (len > slen - start)
		len = slen - start;
	subs = (char *)malloc(sizeof(char) * (len + 1));
	if (!subs)
		return (NULL);
	ft_memcpy(subs, s + start, len);
	subs[len] = '\0';
	return (subs);
}
