#include "../../include/minishell.h"

/**
 * @brief Handles built-in commands that require redirections.
 * 
 * This function checks if the given command is a built-in command that requires 
 * handling of redirections (`exit`, `cd`, `export`, `unset`). If so, it processes 
 * the command accordingly.
 * 
 * @param command The command structure containing arguments and redirections.
 * @param data The main shell data structure.
 * @return true If the command was a built-in and was handled.
 * @return false If the command was not a relevant built-in.
 */
static bool	handle_builtins_with_redirs(t_command *command, t_data *data)
{
	if (ft_strncmp(command->args[0], "exit", 5) == 0)
	{
		if (check_redirs(command) == true)
			return (ft_exit(data, command), true);
		return (true);
	}
	if (ft_strncmp(command->args[0], "cd", 3) == 0)
	{
		if (check_redirs(command) == true)
			return (ft_cd(command, data), true);
		return (true);
	}
	if (ft_strncmp(command->args[0], "export", 7) == 0)
	{
		if (check_redirs(command) == true)
			return (ft_export(command, data), true);
		return (true);
	}
	if (ft_strncmp(command->args[0], "unset", 6) == 0)
	{
		if (check_redirs(command) == true)
			return (ft_unset(command, data), true);
		return (true);
	}
	return (false);
}

/**
 * @brief Handles history-related built-in commands.
 * 
 * This function processes the `history` built-in command and its options:
 * - `history` (prints command history)
 * - `history -c` (clears history)
 * - `history -cw` (clears and deletes history file)
 * 
 * @param command The command structure containing arguments.
 * @param data The main shell data structure.
 * @return true If the history command was recognized and executed.
 * @return false If the command was not a history-related built-in.
 */
static bool	handle_history_builtins(t_command *command, t_data *data)
{
	if (ft_strncmp(command->args[0], "history", 8) == 0 && !command->args[1])
		return (print_history(data), true);
	if (ft_strncmp(command->args[0], "history", 8) == 0
		&& ft_strncmp(command->args[1], "-c", 2) == 0
		&& command->args[1][2] != 'w')
		return (rl_clear_history(), true);
	if (ft_strncmp(command->args[0], "history", 8) == 0
		&& ft_strncmp(command->args[1], "-cw", 4) == 0)
		return (delete_history_file(data), true);
	return (false);
}

/**
 * @brief Checks if a command is a built-in that should be executed before pipes.
 * 
 * This function determines whether a command is a built-in that needs to be handled
 * immediately before the piping process starts. It delegates the check to helper 
 * functions for built-ins with redirections and history-related commands.
 * 
 * @param command The command structure containing arguments.
 * @param data The main shell data structure.
 * @return true If the command is a built-in and was executed.
 * @return false If the command is not a built-in or does not require pre-pipe execution.
 */
bool	check_builtin_prepipe(t_command *command, t_data *data)
{
	if (!command->args)
		return (false);
	if (handle_builtins_with_redirs(command, data))
		return (true);
	if (handle_history_builtins(command, data))
		return (true);
	return (false);
}
