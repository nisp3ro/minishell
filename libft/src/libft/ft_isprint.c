#include "../../include/libft.h"

/**
 * @brief Checks if a character is printable.
 *
 * This function reimplements the standard `isprint` function.
 * It determines whether the given character is a printable ASCII character,
 * including space (32) and all visible characters up to 126.
 *
 * @param c The character to check (passed as an int, but typically
 *          expected to be an unsigned char or EOF).
 * @return 1 if `c` is a printable character, 0 otherwise.
 */
int	ft_isprint(int c)
{
	if (c >= 32 && c <= 126)
		return (1);
	return (0);
}
