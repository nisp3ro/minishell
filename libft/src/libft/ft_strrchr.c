#include "../../include/libft.h"

/**
 * @brief Locates the last occurrence of a character in a string.
 *
 * Searches for the last occurrence of the character specified by tofind (converted
 * to a char) in the null-terminated string pointed to by str. The terminating null
 * byte is considered part of the string, so if tofind is `\0`, a pointer to the terminator
 * is returned.
 *
 * @param str Pointer to the null-terminated string to be searched.
 * @param tofind The character to locate (as an int, internally converted to char).
 * @return char* A pointer to the last occurrence of the character in the string, or
 *               NULL if the character is not found.
 */
char	*ft_strrchr(const char *str, int tofind)
{
	int	i;

	i = ft_strlen(str);
	while (i >= 0)
	{
		if (str[i] == (char)tofind)
			return ((char *)&str[i]);
		i--;
	}
	return (NULL);
}
