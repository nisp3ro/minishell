#include "../../include/libft.h"

/**
 * @brief Checks if a character is an ASCII character.
 *
 * This function reimplements the standard `isascii` function.
 * It verifies whether the given character falls within the 
 * ASCII range (0-127).
 *
 * @param c The character to check (passed as an int, but typically 
 *          expected to be an unsigned char or EOF).
 * @return 1 if `c` is a valid ASCII character, 0 otherwise.
 */
int	ft_isascii(int c)
{
	if (c >= 0 && c <= 127)
		return (1);
	return (0);
}
