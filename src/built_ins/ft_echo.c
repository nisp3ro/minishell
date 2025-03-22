#include "../../include/minishell.h"

/**
 * @brief Mimics the behavior of the echo command.
 * 
 * - Supports the `-n` flag to suppress the newline.
 * - Prints each argument separated by a space.
 * 
 * @param data Pointer to the shell data structure.
 * @param command The parsed command containing arguments.
 */
void	ft_echo(t_data *data, t_command *command)
{
	int	i;
	bool	suppress_newline;

	i = 1;
	suppress_newline = false;

	// Handle multiple "-n" flags
	while (command->args[i] && ft_strncmp(command->args[i], "-n", 3) == 0)
	{
		suppress_newline = true;
		i++;
	}

	// Print arguments
	while (command->args[i])
	{
		write(STDOUT_FILENO, command->args[i], ft_strlen(command->args[i]));
		if (command->args[i + 1])
			write(STDOUT_FILENO, " ", 1);
		i++;
	}

	// Print newline unless suppressed
	if (!suppress_newline)
		write(STDOUT_FILENO, "\n", 1);

	data->g_exit_code = 0;
}
