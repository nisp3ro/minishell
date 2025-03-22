#include "../../include/libft.h"

/**
 * @brief Duplicates a string.
 *
 * Allocates sufficient memory for a copy of the string src, copies the string into
 * the allocated memory, and returns a pointer to it. The duplicated string is
 * null-terminated.
 *
 * @param src The source string to duplicate.
 * @return char* Pointer to the newly allocated duplicate string, or NULL if memory
 *               allocation fails.
 */
char	*ft_strdup(const char *src)
{
	size_t	len;
	char	*dup;

	len = ft_strlen(src);
	dup = (char *)malloc(sizeof(char) * (len + 1));
	if (dup == NULL)
		return (NULL);
	ft_memcpy(dup, src, len);
	dup[len] = '\0';
	return (dup);
}
