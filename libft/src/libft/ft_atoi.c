#include "../../include/libft.h"

/**
 * @brief Converts a string to an integer.
 *
 * This function parses the given string, skipping any leading whitespace, and converts
 * the subsequent characters into an integer. It handles an optional '+' or '-' sign.
 * The conversion is performed by accumulating the result as a negative value to
 * avoid potential overflow issues, then adjusting the sign at the end.
 *
 * @param str The input string containing the representation of an integer.
 * @return int The converted integer.
 */
int	ft_atoi(const char *str)
{
	int	i;
	int	result;
	int	sign;

	i = 0;
	result = 0;
	sign = -1;
	// Skip leading whitespace characters (tabs, newlines, spaces, etc.)
	while (ft_isspace(str[i]))
		i++;
	// Check for optional sign indicator
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = 1;
		i++;
	}
	// Convert digits to integer by accumulating the result
	while (ft_isdigit(str[i]))
	{
		result = (result * 10) - (str[i] - '0');
		i++;
	}
	return (result * sign);
}
