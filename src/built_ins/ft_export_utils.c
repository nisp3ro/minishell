#include "../../include/minishell.h"

/**
 * @brief Checks if a given string is a valid identifier.
 *
 * This function verifies that the provided string conforms to the rules of a valid
 * identifier. A valid identifier must not start with a digit or an '=' character,
 * and it can only contain alphanumeric characters or underscores until an '=' is encountered.
 *
 * @param str The string to validate.
 * @return int Returns OK if the identifier is valid, or ERROR otherwise.
 */
int	is_valid_identifier(const char *str)
{
	int	i;

	i = 0;
	if (ft_isdigit(str[0]) || str[0] == '=')
		return (ERROR);
	while (str[i] != '\0' && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (ERROR);
		i++;
	}
	return (OK);
}

/**
 * @brief Prints all exported variables in a formatted manner.
 *
 * This function iterates through the exported variables list (data->exp_vars)
 * and prints each variable in the format:
 * "declare -x VARIABLE_NAME="VARIABLE_VALUE""
 *
 * @param data Pointer to the minishell data structure containing the exported variables.
 */
void	print_exported_vars(t_data *data)
{
	t_vars	*tmp;

	tmp = data->exp_vars;
	while (tmp != NULL)
	{
		write(STDOUT_FILENO, "declare -x ", 11);
		write(STDOUT_FILENO, tmp->name, ft_strlen(tmp->name));
		write(STDOUT_FILENO, "=\"", 2);
		write(STDOUT_FILENO, tmp->value, ft_strlen(tmp->value));
		write(STDOUT_FILENO, "\"\n", 2);
		tmp = tmp->next;
	}
}

/**
 * @brief Handles an invalid identifier error.
 *
 * Prints an error message to STDERR indicating that the identifier is invalid,
 * sets the global exit code to 1, and increments the provided index.
 *
 * @param data Pointer to the minishell data structure.
 * @param i Pointer to an index that is incremented to skip the invalid identifier.
 */
void	handle_invalid_identifier(t_data *data, int *i)
{
	write(STDERR_FILENO, " not a valid identifier\n", 24);
	data->g_exit_code = 1;
	(*i)++;
}
