#include "../../include/minishell.h"

/**
 * @brief Creates an environment entry string.
 *
 * Constructs a string in the format "name=value" by allocating sufficient memory,
 * copying the name, appending an equal sign, and then the value.
 *
 * @param name The name of the environment variable.
 * @param value The value of the environment variable.
 * @return char* A pointer to the newly created environment entry string, or NULL on failure.
 */
char	*create_env_entry(char *name, char *value)
{
	size_t	size;
	char	*entry;

	size = ft_strlen(name) + ft_strlen(value) + 2;
	entry = malloc(size);
	if (!entry)
		return (NULL);
	ft_strcpy(entry, name);
	ft_strcat(entry, "=");
	ft_strcat(entry, value);
	return (entry);
}

/**
 * @brief Replaces the value of a user-defined variable.
 *
 * Iterates through the linked list of environment variables and, when a variable with the
 * matching name is found, frees its current value and replaces it with a duplicate of the new value.
 * If the variable is not found in the exported variables list, it adds it using set_exp().
 *
 * @param env_vars Double pointer to the head of the environment variables linked list.
 * @param name The name of the variable to replace.
 * @param value The new value to assign to the variable.
 * @param data Pointer to the minishell data structure.
 */
void	replace_user_variable(t_vars **env_vars, char *name, char *value,
		t_data *data)
{
	t_vars	*current;

	current = *env_vars;
	while (current != NULL)
	{
		if (current->name && ft_strncmp(current->name, name, (ft_strlen(name)
					+ 1)) == 0)
		{
			free(current->value);
			current->value = ft_strdup(value);
			if (!current->value)
			{
				perror("malloc");
				exit(1);
				break ;
			}
		}
		current = current->next;
	}
	if (!mini_getvars(data->exp_vars, name))
		set_exp(data, name, value);
}

/**
 * @brief Processes an environment variable assignment.
 *
 * Searches the environment array (data->envp) for an entry that matches the given name.
 * If found, it replaces the existing entry with a new one created from the name and value.
 *
 * @param name The name of the variable.
 * @param value The value of the variable.
 * @param data Pointer to the minishell data structure.
 * @return bool Returns true if the variable was processed (found and replaced), false otherwise.
 */
bool	process_environment_variable(char *name, char *value, t_data *data)
{
	int	i;

	i = 0;
	while (data->envp[i] != NULL)
	{
		if (ft_strncmp(data->envp[i], name, ft_strlen(name)) == 0
			&& data->envp[i][ft_strlen(name)] == '=')
		{
			free(data->envp[i]);
			data->envp[i] = create_env_entry(name, value);
			if (!data->envp[i])
			{
				perror("malloc");
				exit(1);
			}
			return (true);
		}
		i++;
	}
	return (false);
}

/**
 * @brief Processes a user variable assignment.
 *
 * Depending on whether the variable exists in the exported variables list or in the
 * regular variables list, this function replaces the variable's value using replace_user_variable()
 * or adds a new variable using set_variable().
 *
 * @param name The name of the variable.
 * @param value The value of the variable.
 * @param env_vars Double pointer to the head of the variables linked list.
 * @param data Pointer to the minishell data structure.
 */
void	process_user_variable(char *name, char *value, t_vars **env_vars,
		t_data *data)
{
	if (mini_getvars(data->exp_vars, name))
	{
		replace_user_variable(&data->exp_vars, name, value, data);
		set_exp(data, name, value);
	}
	else if (mini_getvars(data->vars, name))
	{
		replace_user_variable(&data->vars, name, value, data);
	}
	else
	{
		set_variable(env_vars, name, value);
	}
}

/**
 * @brief Handles a variable assignment from the input.
 *
 * Expands any variables in the input assignment string, splits the string at the first '='
 * into a name and a value, and then processes the assignment by updating both the environment
 * and user-defined variables.
 *
 * @param input The input string containing the variable assignment.
 * @param env_vars Double pointer to the head of the variables linked list.
 * @param data Pointer to the minishell data structure.
 * @return bool Returns true on success, or false on failure.
 */
bool	handle_variable_assignment(char *input, t_vars **env_vars, t_data *data)
{
	char	*expanded;
	char	*equal_sign;
	char	*name;
	char	*value;

	expanded = expand_variables(input, data);
	if (!expanded)
		return (perror("Error"), false);
	equal_sign = ft_strchr(expanded, '=');
	if (!equal_sign)
		return (free(expanded), false);
	*equal_sign = '\0';
	name = expanded;
	value = equal_sign + 1;
	process_environment_variable(name, value, data);
	process_user_variable(name, value, env_vars, data);
	free(expanded);
	return (true);
}
