#include "../../include/libft.h"

/**
 * @brief Writes an integer to a file descriptor.
 *
 * This function converts the integer `n` to its string representation and writes it
 * to the given file descriptor `fd`. It handles negative numbers by writing a '-' sign
 * before the digits.
 *
 * @param n The integer to output.
 * @param fd The file descriptor on which to write the integer.
 */
void	ft_putnbr_fd(int n, int fd)
{
	unsigned int	nb;

	if (n < 0)
	{
		ft_putchar_fd('-', fd);
		nb = -n;
	}
	else
		nb = n;
	if (nb > 9)
		ft_putnbr_fd(nb / 10, fd);
	ft_putchar_fd((nb % 10) + '0', fd);
}
