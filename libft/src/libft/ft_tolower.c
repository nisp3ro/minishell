#include "../../include/libft.h"

/**
 * @brief Converts an uppercase letter to lowercase.
 *
 * This function checks if the given character is an uppercase letter ('A' to 'Z')
 * and, if so, converts it to its lowercase equivalent by adding 32 to its ASCII value.
 * If the character is not an uppercase letter, it is returned unchanged.
 *
 * @param ch The character to convert.
 * @return int The lowercase equivalent of ch if ch is uppercase; otherwise, ch.
 */
int	ft_tolower(int ch)
{
	if (ch >= 'A' && ch <= 'Z')
		return (ch + 32);
	return (ch);
}
