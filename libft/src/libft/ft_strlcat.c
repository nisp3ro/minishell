#include "../../include/libft.h"

/**
 * @brief Concatenates two strings with size limitation.
 *
 * This function appends the NUL-terminated string src to the end of dst.
 * It will append at most (dsize - strlen(dst) - 1) bytes, NUL-terminating the
 * result (as long as dsize is greater than strlen(dst)).
 *
 * @param dst Destination string, which must be NUL-terminated.
 * @param src Source string to be appended to dst.
 * @param dsize Full size of the destination buffer.
 * @return size_t The total length of the string it tried to create (i.e., the
 *                initial length of dst plus the length of src). If the return
 *                value is >= dsize, truncation occurred.
 */
size_t	ft_strlcat(char *dst, const char *src, size_t dsize)
{
	size_t	i;
	size_t	dlen;
	size_t	slen;

	dlen = ft_strlen(dst);
	slen = ft_strlen(src);
	i = 0;
	if (dsize == 0)
		return (slen);
	if (dsize <= dlen)
		return (slen + dsize);
	while (src[i] != '\0' && (dlen + i) < (dsize - 1))
	{
		dst[dlen + i] = src[i];
		i++;
	}
	dst[dlen + i] = '\0';
	return (dlen + slen);
}
