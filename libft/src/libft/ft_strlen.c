#include "../../include/libft.h"

/**
 * @brief Computes the length of a null-terminated string.
 *
 * This function is a reimplementation of the standard `strlen` function.
 * It counts the number of characters in the given string until it reaches
 * the null terminator (`'\0'`).
 *
 * @note If `str` is `NULL`, the function will result in undefined behavior
 * (most likely causing a segmentation fault).
 *
 * @param str A pointer to the null-terminated string.
 * @return The length of the string (number of characters before `'\0'`).
 */
size_t	ft_strlen(const char *str)
{
	size_t	len;

	len = 0;
	while (str[len])
		len++;
	return (len);
}
