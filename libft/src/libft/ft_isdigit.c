#include "../../include/libft.h"

/**
 * @brief Checks if a character is a decimal digit (0-9).
 *
 * This function reimplements the standard `isdigit` function.
 * It determines whether the given character is a numeric digit
 * from '0' to '9' (ASCII values 48-57).
 *
 * @param c The character to check (passed as an int, but typically 
 *          expected to be an unsigned char or EOF).
 * @return 1 if `c` is a digit (0-9), 0 otherwise.
 */
int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}
