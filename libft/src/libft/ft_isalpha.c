#include "../../include/libft.h"

/**
 * @brief Checks if a character is an alphabetic letter.
 *
 * This function reimplements the standard `isalpha` function.
 * It verifies whether the given character is a letter (either uppercase 
 * or lowercase) based on the ASCII character set.
 *
 * @param c The character to check (passed as an int, but typically 
 *          expected to be an unsigned char or EOF).
 * @return 1 if `c` is an alphabetic character, 0 otherwise.
 */
int	ft_isalpha(int c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
		return (1);
	return (0);
}
