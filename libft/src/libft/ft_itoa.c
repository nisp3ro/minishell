#include "../../include/libft.h"

/**
 * @brief Computes the number of digits in an unsigned integer.
 *
 * This function calculates the number of digits required to represent the
 * given unsigned integer in decimal notation.
 *
 * @param n The unsigned integer.
 * @return int The number of digits.
 */
static int	ft_numlen(unsigned int n)
{
	int	len = 1;
	while (n > 9)
	{
		n /= 10;
		len++;
	}
	return (len);
}

/**
 * @brief Converts an integer to a null-terminated string.
 *
 * Allocates (with malloc) and returns a string representing the integer n.
 * Negative numbers are properly handled by prepending a '-' sign.
 *
 * @param n The integer to convert.
 * @return char* The string representation of the integer, or NULL if memory allocation fails.
 */
char	*ft_itoa(int n)
{
	unsigned int	nb;
	int				sign;
	int				len;
	char			*res;
	int				i;

	/* Determine the sign and absolute value */
	sign = (n < 0);
	if (sign)
		nb = -n;
	else
		nb = n;
	/* Compute number of digits in nb */
	len = ft_numlen(nb);
	/* Allocate memory for sign + digits + null terminator */
	res = (char *)malloc(sizeof(char) * (sign + len + 1));
	if (!res)
		return (NULL);
	/* Set the index for the last digit */
	i = sign + len;
	/* Fill in the digits from the end */
	while (--i >= sign)
	{
		res[i] = (nb % 10) + '0';
		nb /= 10;;
	}
	/* If negative, add the '-' sign */
	if (sign)
		res[0] = '-';
	res[sign + len] = '\0';
	return (res);
}
