#include "../../include/libft.h"

/**
 * @brief Copies a string into a sized buffer.
 *
 * This function copies up to (dsize - 1) characters from the NUL-terminated
 * string src to dst, NUL-terminating the result if dsize is not 0.
 * It returns the total length of the string src.
 *
 * @param dst Destination buffer.
 * @param src Source string.
 * @param dsize Size of the destination buffer.
 * @return size_t Length of src.
 */
size_t	ft_strlcpy(char *dst, const char *src, size_t dsize)
{
	size_t	i;
	size_t	len;

	len = ft_strlen(src);
	if (dsize == 0)
		return (len);
	i = 0;
	while (src[i] != '\0' && i < (dsize - 1))
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (len);
}
