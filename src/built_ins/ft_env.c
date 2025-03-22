#include "../include/minishell.h"

/**
 * @brief Prints the environment variables.
 *
 * This function iterates through the environment variable array stored in the minishell
 * data structure and prints each variable that contains an '=' character. After printing,
 * it sets the global exit code to 0.
 *
 * @param data Pointer to the minishell data structure.
 */
void	ft_env(t_data *data)
{
	int	i;

	i = 0;
	while (data->envp[i])
	{
		if (ft_strchr(data->envp[i], '='))
		{
			write(STDOUT_FILENO, data->envp[i], ft_strlen(data->envp[i]));
			write(STDOUT_FILENO, "\n", 1);
		}
		i++;
	}
	data->g_exit_code = 0;
}
