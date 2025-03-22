#include "../../include/libft.h"

/**
 * @brief Locates the first occurrence of a character in a string.
 *
 * Searches for the first occurrence of the character specified by tofind (converted
 * to a char) in the string pointed to by str. The terminating null byte is considered
 * part of the string, so if tofind is `\0`, a pointer to the terminator is returned.
 *
 * @param str Pointer to the null-terminated string to be searched.
 * @param tofind The character to locate (as an int, but internally converted to char).
 * @return char* A pointer to the first occurrence of the character in the string, or
 *               NULL if the character is not found.
 */
char	*ft_strchr(const char *str, int tofind)
{
	while (*str && *str != (char)tofind)
		str++;
	if (*str == (char)tofind)
		return ((char *)str);
	return (NULL);
}
