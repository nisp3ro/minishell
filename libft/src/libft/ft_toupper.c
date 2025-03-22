#include "../../include/libft.h"

/**
 * @brief Converts a lowercase letter to uppercase.
 *
 * This function checks if the given character is a lowercase letter ('a' to 'z')
 * and, if so, converts it to its uppercase equivalent. If the character is not a
 * lowercase letter, it is returned unchanged.
 *
 * @param ch The character to convert.
 * @return int The uppercase equivalent of ch if ch is lowercase, otherwise ch.
 */
int	ft_toupper(int ch)
{
	if (ch >= 'a' && ch <= 'z')
		return (ch - 32);
	return (ch);
}
