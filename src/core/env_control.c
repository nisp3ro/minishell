#include "../../include/minishell.h"

/**
 * @brief Specially sets an export variable in the environment.
 *
 * This function adds a new variable to the environment (data->envp) only if it 
 * does not already exist. The new variable is added as a standalone string (without an '=' sign).
 * The environment array is reallocated to accommodate the new variable.
 *
 * @param data Pointer to the minishell data structure.
 * @param name The name of the variable to add.
 * @return int Returns OK if the variable was successfully added or already exists.
 */
int	special_set_exp(t_data *data, char *name)
{
	int	i;

	i = 0;
	if (mini_getenv(name, data->envp) != NULL)
		return (OK);
	while (data->envp[i])
		i++;
	data->envp = ft_realloc(data->envp, sizeof(char *) * (i + 2));
	data->envp[i] = ft_strdup(name);
	if (data->envp[i] == NULL)
	{
		perror("malloc");
		exit(1);
	}
	data->envp[i + 1] = NULL;
	return (OK);
}

/**
 * @brief Sets an export variable in the environment with a value.
 *
 * This function adds a new variable to the environment (data->envp) in the format "name=value"
 * if it does not already exist. The environment array is reallocated to accommodate the new variable.
 *
 * @param data Pointer to the minishell data structure.
 * @param name The name of the variable.
 * @param value The value of the variable.
 * @return int Returns OK if the variable was successfully added or already exists.
 */
int	set_exp(t_data *data, char *name, char *value)
{
	int	i;

	i = 0;
	if (mini_getenv(name, data->envp) != NULL)
		return (OK);
	while (data->envp[i])
		i++;
	data->envp = ft_realloc(data->envp, sizeof(char *) * (i + 2));
	data->envp[i] = malloc(ft_strlen(name) + ft_strlen(value) + 2);
	if (data->envp[i] == NULL)
	{
		perror("malloc");
		exit(1);
	}
	ft_strcpy(data->envp[i], name);
	ft_strcat(data->envp[i], "=");
	ft_strcat(data->envp[i], value);
	data->envp[i + 1] = NULL;
	return (OK);
}

/**
 * @brief Updates the last command argument in the environment.
 *
 * This function searches for an environment variable with the given name (e.g., "_") and updates its value.
 * If the variable is found, it frees the old string and allocates a new one in the format "name=value".
 * If the variable is not found, it creates the new variable using set_exp.
 *
 * @param data Pointer to the minishell data structure.
 * @param name The name of the environment variable to update.
 * @param value The new value to assign to the variable.
 */
void	set_last_cmd_arg(t_data *data, char *name, char *value)
{
	int	i;

	i = 0;
	while (data->envp[i] != NULL)
	{
		if (ft_strncmp(data->envp[i], name, ft_strlen(name)) == 0
			&& data->envp[i][ft_strlen(name)] == '=')
		{
			free(data->envp[i]);
			data->envp[i] = malloc(ft_strlen(name) + ft_strlen(value) + 2);
			if (data->envp[i] == NULL)
			{
				perror("malloc");
				exit(1);
			}
			ft_strcpy(data->envp[i], name);
			ft_strcat(data->envp[i], "=");
			ft_strcat(data->envp[i], value);
			return ;
		}
		i++;
	}
	set_exp(data, name, value);
}
