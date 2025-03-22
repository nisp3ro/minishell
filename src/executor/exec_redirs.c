#include "../../include/minishell.h"

/**
 * @brief Processes a single redirection for a command.
 *
 * Depending on the type of redirection (INPUT or OUTPUT), this function updates the
 * corresponding file descriptor in the provided redirection variables structure.
 * For input redirection, it opens the specified file in read-only mode. For output
 * redirection, it opens the file in write-only mode with either append or truncate
 * flags based on the command's append flag. On error, it prints an error message and exits.
 *
 * @param command Pointer to the command structure containing redirection information.
 * @param red Pointer to the redirection variables structure that holds file descriptors and flags.
 */
void	redir(t_command *command, t_redir_vars *red)
{
	if (command->redir->type == INPUT)
	{
		red->input_redirection = true;
		if (red->fd_in != STDIN_FILENO)
			close(red->fd_in);
		red->fd_in = open(command->redir->value, O_RDONLY);
		if (red->fd_in < 0)
			return (perror(" "), exit(EXIT_FAILURE));
	}
	else if (command->redir->type == OUTPUT)
	{
		red->output_redirection = true;
		if (red->fd_out != STDOUT_FILENO)
			close(red->fd_out);
		if (command->append)
			red->fd_out = open(command->redir->value,
					O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			red->fd_out = open(command->redir->value,
					O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (red->fd_out < 0)
			return (perror(" "), exit(EXIT_FAILURE));
	}
	command->redir = command->redir->next;
}

/**
 * @brief Initializes the redirection variables.
 *
 * This function sets the default file descriptors for input and output to STDIN and STDOUT,
 * and initializes the redirection flags to false.
 *
 * @param red Pointer to the redirection variables structure to initialize.
 */
void	init_redir_vars(t_redir_vars *red)
{
	red->fd_in = STDIN_FILENO;
	red->fd_out = STDOUT_FILENO;
	red->input_redirection = false;
	red->output_redirection = false;
}

/**
 * @brief Applies the redirections using dup2.
 *
 * If input or output redirection has been set, this function duplicates the corresponding
 * file descriptor to STDIN_FILENO or STDOUT_FILENO and then closes the original descriptor.
 *
 * @param red Pointer to the redirection variables structure containing redirection settings.
 */
void	manage_redirections(t_redir_vars *red)
{
	if (red->input_redirection)
	{
		dup2(red->fd_in, STDIN_FILENO);
		close(red->fd_in);
	}
	if (red->output_redirection)
	{
		dup2(red->fd_out, STDOUT_FILENO);
		close(red->fd_out);
	}
}

/**
 * @brief Manages redirections for a command and returns the command path.
 *
 * This function first initializes redirection variables and determines the command path.
 * If the command contains a '/' in its arguments, a custom path is created;
 * otherwise, the command is searched in the PATH. It then applies any inherited input redirection
 * from the pipeline, processes each redirection associated with the command, and finally,
 * if there is a next command in the pipeline, sets up the output redirection accordingly.
 * After processing all redirections, it applies them using manage_redirections() and returns
 * the final command path.
 *
 * @param data Pointer to the minishell data structure.
 * @param command Pointer to the command structure.
 * @param envp The environment variable array.
 * @param pip Pointer to the pipeline variables structure.
 * @return char* The full path of the command to be executed.
 */
char	*manage_redirs(t_data *data, t_command *command, char **envp,
			t_pip_vars *pip)
{
	t_redir_vars	red;

	init_redir_vars(&red);
	if (command->args && ft_strchr(command->args[0], '/') != 0)
		ft_create_custom_path(&red.command_path, command);
	else if (command->args)
		red.command_path = find_command_in_path(data, command->args[0], envp);
	if (pip->in_fd != STDIN_FILENO)
	{
		dup2(pip->in_fd, STDIN_FILENO);
		close(pip->in_fd);
	}
	while (command->redir)
		redir(command, &red);
	if (command->next)
	{
		dup2(pip->pipefd[1], STDOUT_FILENO);
		close(pip->pipefd[1]);
		close(pip->pipefd[0]);
	}
	manage_redirections(&red);
	return (red.command_path);
}
