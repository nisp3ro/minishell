#include "../../include/minishell.h"

/**
 * @brief Prints the current working directory.
 *
 * This function writes the current working directory stored in data->pwd to the
 * standard output. If data->pwd is NULL, an error message is printed to standard error,
 * and the global exit code is set to 1.
 *
 * @param data Pointer to the minishell data structure.
 */
void	ft_pwd(t_data *data)
{
	if (data->pwd == NULL)
	{
		write(STDERR_FILENO, "pwd: cannot get the current directory", 37);
		data->g_exit_code = 1;
		return ;
	}
	write(STDOUT_FILENO, data->pwd, ft_strlen(data->pwd));
	write(STDOUT_FILENO, "\n", 1);
	data->g_exit_code = 0;
}
