#include "../include/minishell.h"

/**
 * @brief Checks if a command is a built-in and executes it.
 * 
 * This function compares the command's first argument with the known built-ins
 * and executes the corresponding function if there is a match.
 * 
 * @param command The command structure containing arguments.
 * @param data The minishell data structure.
 * @return true If the command is a built-in and was executed.
 * @return false If the command is not a built-in.
 */
bool	check_builtin(t_command *command, t_data *data)
{
	if (!command->args)
		return (false);
	if (ft_strncmp(command->args[0], "exit", 5) == 0)
		return (ft_exit(data, command), true);
	if (ft_strncmp(command->args[0], "echo", 5) == 0)
		return (ft_echo(data, command), true);
	if (ft_strncmp(command->args[0], "cd", 3) == 0)
		return (ft_cd(command, data), true);
	if (ft_strncmp(command->args[0], "pwd", 4) == 0)
		return (ft_pwd(data), true);
	if (ft_strncmp(command->args[0], "export", 7) == 0)
		return (ft_export(command, data), true);
	if (ft_strncmp(command->args[0], "unset", 6) == 0)
		return (ft_unset(command, data), true);
	if (ft_strncmp(command->args[0], "env", 4) == 0)
		return (ft_env(data), true);
	if (ft_strncmp(command->args[0], "history", 8) == 0 && !command->args[1])
		return (print_history(data), true);
	if (ft_strncmp(command->args[0], "history", 8) == 0
		&& ft_strncmp(command->args[1], "-c", 3) == 0)
		return (rl_clear_history(), true);
	if (ft_strncmp(command->args[0], "history", 8) == 0
		&& ft_strncmp(command->args[1], "-cw", 4) == 0)
		return (delete_history_file(data), true);
	return (false);
}
