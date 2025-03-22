#include "../../include/libft.h"

/**
 * @brief Writes a character to a file descriptor.
 *
 * Outputs the character 'c' to the given file descriptor 'fd'.
 *
 * @param c The character to write.
 * @param fd The file descriptor on which to write the character.
 */
void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}
