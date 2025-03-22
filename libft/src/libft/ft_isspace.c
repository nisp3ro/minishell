#include "../../include/libft.h"

/**
 * @brief Checks if a character is a whitespace character.
 *
 * This function checks if the given character (passed as an int) is a whitespace character.
 * The following characters are considered whitespace:
 *  - Space (' ')
 *  - Form feed ('\f')
 *  - Newline ('\n')
 *  - Carriage return ('\r')
 *  - Horizontal tab ('\t')
 *  - Vertical tab ('\v')
 *
 * @param c The character to check.
 * @return int Returns 1 if c is a whitespace character, otherwise returns 0.
 */
int	ft_isspace(int c)
{
	if (c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t'
		|| c == '\v')
		return (1);
	return (0);
}
