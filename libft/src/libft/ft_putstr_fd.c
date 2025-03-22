#include "../../include/libft.h"

/**
 * @brief Writes a string to a file descriptor.
 *
 * This function outputs the given null-terminated string `s` to the file descriptor `fd`.
 * If the string is NULL, the function does nothing.
 *
 * @param s The string to be written.
 * @param fd The file descriptor on which to write the string.
 */
void	ft_putstr_fd(char *s, int fd)
{
	int	i;

	if (!s)
		return;
	i = 0;
	while (s[i])
	{
		ft_putchar_fd(s[i], fd);
		i++;
	}
}
