#include "../../include/minishell.h"

/**
 * @brief Checks if a character is a delimiter.
 * 
 * Delimiters are considered to be spaces, tabs, pipes (`|`), and redirections (`<`, `>`).
 * 
 * @param c The character to check.
 * @return int Returns 1 if the character is a delimiter, 0 otherwise.
 */
int	is_delimiter(char c)
{
	return (c == '\t' || c == ' ' || c == '|' || c == '<' || c == '>');
}

/**
 * @brief Checks if a character is a quote.
 * 
 * Supports both single (`'`) and double (`"`) quotes.
 * 
 * @param c The character to check.
 * @return true If the character is a quote.
 * @return false Otherwise.
 */
bool	is_quote(char c)
{
	if (c == '\'' || c == '\"')
		return (true);
	return (false);
}

/**
 * @brief Checks if a string consists only of whitespace characters.
 * 
 * This function iterates through the string and verifies if all characters 
 * are considered whitespace using `ft_isspace`.
 * 
 * @param str The input string.
 * @return true If the string contains only spaces or is empty.
 * @return false If at least one non-whitespace character is found.
 */
bool	is_all_spaces(char *str)
{
	while (*str)
	{
		if (!ft_isspace(*str))
			return (false);
		str++;
	}
	return (true);
}
