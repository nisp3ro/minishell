#include "../../include/libft.h"

/**
 * @brief Writes a string followed by a newline to a file descriptor.
 *
 * This function outputs the given null-terminated string `s` to the file descriptor `fd`
 * followed by a newline character. It calls `ft_putstr_fd` to write the string and then
 * `ft_putchar_fd` to write the newline.
 *
 * @param s The string to write.
 * @param fd The file descriptor on which to write the string.
 */
void	ft_putendl_fd(char *s, int fd)
{
	ft_putstr_fd(s, fd);
	ft_putchar_fd('\n', fd);
}
