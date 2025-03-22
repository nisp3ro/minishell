#include "../../include/minishell.h"

/**
 * @brief Initializes the search command variables.
 *
 * This function sets the initial values for the t_search_command structure.
 * It sets the command_path to NULL and the index (i) to 0.
 *
 * @param vars Pointer to the t_search_command structure to initialize.
 */
void	init_search_command_vars(t_search_command *vars)
{
	vars->command_path = NULL;
	vars->i = 0;
}

/**
 * @brief Creates a custom path for a command.
 *
 * This function modifies the command's argument pointer to point to the substring 
 * starting from the last '/' character in the original path. It assigns the original 
 * command's first argument to *path, calculates its length, and adjusts the pointer 
 * accordingly.
 *
 * @param path Pointer to a string where the custom path will be stored.
 * @param command Pointer to the command structure containing the arguments.
 */
void	ft_create_custom_path(char **path, t_command *command)
{
	int	i;
	int	len;

	*path = command->args[0];
	len = ft_strlen(command->args[0]);
	i = len - 1;
	while (i >= 0 && command->args[0][i] != '/')
		i--;
	command->args[0] += i;
}

/**
 * @brief Checks if the command has valid arguments.
 *
 * This function verifies that the command structure has at least one argument
 * and that the first argument is not an empty string.
 *
 * @param command Pointer to the command structure.
 * @return true If the command has valid arguments.
 * @return false Otherwise.
 */
bool	check_cmd_args(t_command *command)
{
	if (command->args && command->args[0] != NULL
		&& command->args[0][0] != '\0')
		return (true);
	return (false);
}

/**
 * @brief Initializes pipeline variables.
 *
 * This function initializes the t_pip_vars structure for managing a pipeline.
 * It sets the pipeline index to 0, the input file descriptor to STDIN_FILENO,
 * stores the pointer to the command list, and waits for a signal (by calling wait_signal(0)).
 *
 * @param pip Pointer to the t_pip_vars structure to initialize.
 * @param command Double pointer to the command list (head of the pipeline).
 */
void	init_pip(t_pip_vars *pip, t_command **command)
{
	pip->i = 0;
	pip->in_fd = STDIN_FILENO;
	pip->command_head = *command;
	wait_signal(0);
}
