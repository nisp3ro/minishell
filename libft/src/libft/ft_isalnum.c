#include "../../include/libft.h"

/**
 * @brief Checks if a character is alphanumeric.
 *
 * This function reimplements the standard `isalnum` function.
 * It determines whether the given character is an alphanumeric character,
 * meaning it is either a letter (A-Z, a-z) or a digit (0-9).
 *
 * @param c The character to check (passed as an int, but typically 
 *          expected to be an unsigned char or EOF).
 * @return 1 if `c` is an alphanumeric character, 0 otherwise.
 */
int	ft_isalnum(int c)
{
	if (ft_isalpha(c) || ft_isdigit(c))
		return (1);
	return (0);
}
