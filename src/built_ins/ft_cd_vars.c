#include "../../include/minishell.h"

/**
 * @brief Updates the value of an exported environment variable.
 * 
 * This function frees the previous value of the variable and assigns it a new one.
 * If `unset_pwd` is set and the key is "OLDPWD", it sets the value to an empty string.
 * 
 * @param tmp_vars Pointer to the variable to be updated.
 * @param key The name of the variable.
 * @param value The new value to assign.
 * @param data Pointer to the minishell data structure.
 */
static void	update_exp_var_value(t_vars *tmp_vars, char *key, char *value,
		t_data *data)
{
	free(tmp_vars->value);
	if (data->unset_pwd && ft_strncmp(key, "OLDPWD", 6) == 0)
		tmp_vars->value = ft_strdup("");
	else
		tmp_vars->value = ft_strdup(value);
	if (!tmp_vars->value)
	{
		perror("malloc");
		exit(1);
	}
}

/**
 * @brief Searches for an exported variable and updates its value.
 * 
 * Iterates through the exported variables (`exp_vars`) and updates the value if the key is found.
 * 
 * @param data Pointer to the minishell data structure.
 * @param key The name of the variable to update.
 * @param value The new value to assign.
 */
static void	find_and_update_exp_var(t_data *data, char *key, char *value)
{
	t_vars	*tmp_vars;

	tmp_vars = data->exp_vars;
	while (tmp_vars)
	{
		if (ft_strncmp(tmp_vars->name, key, ft_strlen(key)) == 0)
		{
			update_exp_var_value(tmp_vars, key, value, data);
			break ;
		}
		tmp_vars = tmp_vars->next;
	}
}

/**
 * @brief Updates an environment variable in `envp` at a specific index.
 * 
 * If `unset_pwd` is set and the key is "OLDPWD", the value is removed.
 * Otherwise, it constructs a new key-value pair and updates the array.
 * 
 * @param data Pointer to the minishell data structure.
 * @param i Index of the variable in `envp`.
 * @param key The name of the variable.
 * @param value The new value to assign.
 */
static void	update_envp_var(t_data *data, int i, char *key, char *value)
{
	char	*tmp;

	tmp = NULL;
	free(data->envp[i]);
	if (data->unset_pwd && ft_strncmp(key, "OLDPWD", 6) == 0)
		data->envp[i] = ft_strdup(key);
	else
	{
		tmp = ft_strjoin(key, "=");
		if (!tmp)
		{
			perror("malloc");
			exit(1);
		}
		data->envp[i] = ft_strjoin(tmp, value);
		free(tmp);
		if (!data->envp[i])
		{
			perror("malloc");
			exit(1);
		}
	}
}

/**
 * @brief Searches for an environment variable in `envp` and updates its value.
 * 
 * Iterates through `envp` to find the key and updates its value if found.
 * 
 * @param data Pointer to the minishell data structure.
 * @param key The name of the variable to update.
 * @param value The new value to assign.
 */
static void	find_and_update_envp(t_data *data, char *key, char *value)
{
	int	i;

	i = 0;
	while (data->envp[i])
	{
		if (ft_strncmp(data->envp[i], key, ft_strlen(key)) == 0)
		{
			update_envp_var(data, i, key, value);
			break ;
		}
		i++;
	}
}

/**
 * @brief Updates both exported variables and environment variables.
 * 
 * Calls `find_and_update_exp_var` and `find_and_update_envp` to ensure both
 * exported and environment variables are updated.
 * 
 * @param data Pointer to the minishell data structure.
 * @param key The name of the variable to update.
 * @param value The new value to assign.
 */
void	update_env_var(t_data *data, char *key, char *value)
{
	find_and_update_exp_var(data, key, value);
	find_and_update_envp(data, key, value);
	if (data->unset_pwd)
		data->unset_pwd = false;
}
