#include "../../include/minishell.h"

/**
 * @brief Creates the exported variables list from the environment.
 *
 * Iterates through the environment variables (starting from index flag) stored in data->envp,
 * duplicates each variable, splits it at the '=' character, and adds it to the exported variables list.
 * If an error occurs during allocation or insertion, the function cleans up and returns ERROR.
 *
 * @param data Pointer to the minishell data structure.
 * @param exp_v Pointer to the exported variables list to be created.
 * @param flag Starting index offset in the environment array.
 * @return int Returns OK on success or ERROR on failure.
 */
static int	create_exp_vars(t_data *data, t_vars **exp_v, int flag)
{
	int		i;
	char	*var;
	char	*equal_sign;

	i = 0;
	while (data->envp[i + flag])
	{
		var = ft_strdup(data->envp[i]);
		if (var == NULL)
			return (ERROR);
		equal_sign = ft_strchr(var, '=');
		*equal_sign = '\0';
		equal_sign++;
		if (set_variable(exp_v, var, equal_sign) == ERROR)
		{
			free(var);
			clean_variables(*exp_v);
			return (ERROR);
		}
		free(var);
		i++;
	}
	return (OK);
}

/**
 * @brief Handles the case when no environment variables are provided.
 *
 * When the provided environment (envp) is empty or invalid, this function creates a new environment,
 * builds the exported variables list, and creates a PATH variable for the environment. It then sorts the
 * exported variables list. In case of any error, it cleans up and returns ERROR.
 *
 * @param data Pointer to the minishell data structure.
 * @return int Returns OK on success or ERROR on failure.
 */
static int	handle_no_env(t_data *data)
{
	if (create_envp(data) == ERROR)
		return (ERROR);
	if (create_exp_vars(data, &data->exp_vars, 1))
		return (clean_mtx(data->envp), ERROR);
	if (create_path_no_env(data) == ERROR)
		return (clean_mtx(data->envp), clean_variables(data->exp_vars), ERROR);
	sort_list(&data->exp_vars, data->exp_vars);
	return (OK);
}

/**
 * @brief Initializes data structure members to NULL or default values.
 *
 * This function sets the pointers in the minishell data structure (exp_vars, envp, vars, home, prompt)
 * to NULL and initializes flags and counters (unset_pwd, fd, hist_size, g_exit_code) to their default values.
 *
 * @param data Pointer to the minishell data structure to initialize.
 */
static void	init_nulls(t_data *data)
{
	data->exp_vars = NULL;
	data->envp = NULL;
	data->vars = NULL;
	data->home = NULL;
	data->prompt = NULL;
	data->unset_pwd = false;
	data->fd = 0;
	data->hist_size = 0;
	data->g_exit_code = 0;
}

/**
 * @brief Initializes the minishell data structure.
 *
 * This function initializes the data structure by first setting default values,
 * then processing the provided environment variables. If the environment is missing or empty,
 * it handles it accordingly using handle_no_env(). Otherwise, it duplicates the environment,
 * updates it, creates the exported variables list, and retrieves the current working directory
 * for both pwd and oldpwd. Finally, it recovers the history.
 *
 * @param data Pointer to the minishell data structure to initialize.
 * @param envp The environment variable array provided to the shell.
 * @return int Returns OK on success or ERROR on failure.
 */
int	init_data(t_data *data, char *envp[])
{
	init_nulls(data);
	if (envp == NULL || envp[0] == NULL || envp[0][0] == '\0')
		handle_no_env(data);
	else
	{
		data->envp = cpy_env(envp);
		if (data->envp == NULL)
			return (ERROR);
		if (update_envp(data) == ERROR)
			return (clean_mtx(data->envp), ERROR);
		if (create_exp_vars(data, &data->exp_vars, 1))
			return (clean_mtx(data->envp), ERROR);
	}
	sort_list(&data->exp_vars, data->exp_vars);
	data->pwd = getcwd(NULL, 0);
	if (!data->pwd)
		return (clean_mtx(data->envp), clean_variables(data->exp_vars), ERROR);
	data->oldpwd = getcwd(NULL, 0);
	if (!data->oldpwd)
		return (clean_mtx(data->envp), clean_variables(data->exp_vars),
			free(data->pwd), ERROR);
	return (ft_recover_history(data), OK);
}
