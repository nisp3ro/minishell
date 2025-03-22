#include "../../include/minishell.h"

/**
 * @brief Creates a pipe if the current command is not the last in the pipeline.
 *
 * If there is a next command, this function attempts to create a pipe and stores
 * the file descriptors in pip->pipefd. If the pipe creation fails, an error is
 * printed and the program exits.
 *
 * @param pip Pointer to the structure holding pipe-related variables.
 * @param command Pointer to the current command in the pipeline.
 */
void	create_pipe_if_needed(t_pip_vars *pip, t_command *command)
{
	if (command->next)
		if (pipe(pip->pipefd) == -1)
			return (perror("pipe"), exit(EXIT_FAILURE));
}

/**
 * @brief Executes the child process for a command in the pipeline.
 *
 * In the child process, this function manages redirections by calling manage_redirs(),
 * retrieves the command path, and then attempts to execute the command using execve().
 * If execve() fails, it calls execve_error_exit() to handle the error.
 *
 * @param command Pointer to the current command structure.
 * @param data Pointer to the minishell data structure.
 * @param envp The environment variable array.
 * @param pip Pointer to the structure holding pipe-related variables.
 */
void	execute_child_process(t_command *command, t_data *data, char **envp,
		t_pip_vars *pip)
{
	pip->command_path = manage_redirs(data, command, envp, pip);
	if ((check_cmd_args(command) && !check_builtin(command, data))
		&& execve(pip->command_path, command->args, envp))
		execve_error_exit(command, pip->command_path);
	exit(data->g_exit_code);
}

/**
 * @brief Handles the parent process tasks after forking a child.
 *
 * In the parent process, if the command has here-document delimiters, it waits
 * for the child process to finish using waitpid(). Then, it calls father_process()
 * to manage file descriptors for the next command in the pipeline.
 *
 * @param data Pointer to the minishell data structure.
 * @param pip Pointer to the structure holding pipe-related variables.
 * @param command Pointer to the current command in the pipeline.
 */
void	handle_parent_process(t_data *data, t_pip_vars *pip, t_command *command)
{
	if (command->eof != NULL)
		waitpid(pip->pid, &data->g_exit_code, 0);
	father_process(pip, command);
}

/**
 * @brief Creates a child process to execute a command in the pipeline.
 *
 * This static function increments the pipe counter, forks a new process, and based
 * on the return value of fork(), calls execute_child_process() in the child process
 * or handle_parent_process() in the parent process. If fork() fails, an error message
 * is printed and the program exits.
 *
 * @param pip Pointer to the structure holding pipe-related variables.
 * @param command Pointer to the current command in the pipeline.
 * @param data Pointer to the minishell data structure.
 * @param envp The environment variable array.
 */
static void	handle_child_creation(t_pip_vars *pip, t_command *command,
		t_data *data, char **envp)
{
	pip->i++;
	pip->pid = fork();
	if (pip->pid == -1)
		return (perror("fork"), exit(EXIT_FAILURE));
	if (pip->pid == 0)
		execute_child_process(command, data, envp, pip);
	else
		handle_parent_process(data, pip, command);
}

/**
 * @brief Executes a pipeline of commands.
 *
 * This function initializes the pipeline variables, then iterates over the commands
 * in the pipeline. For each command, if there are here-document delimiters, it processes
 * them by calling here_doc(). It then creates pipes as needed, forks child processes
 * to execute commands, and finally waits for all child processes to finish by calling
 * wait_exit().
 *
 * @param command Pointer to the head of the command list in the pipeline.
 * @param data Pointer to the minishell data structure.
 * @param envp The environment variable array.
 */
void	execute_pipeline(t_command *command, t_data *data, char **envp)
{
	t_pip_vars	pip;
	int			here_doc_pipe[2];

	init_pip(&pip, &command);
	while (command != NULL)
	{
		if (command->eof != NULL)
		{
			handle_here_doc(command, data, here_doc_pipe);
			pip.in_fd = here_doc_pipe[0];
			if (data->g_exit_code == 130)
				break ;
		}
		create_pipe_if_needed(&pip, command);
		if (command->args && command->args[0])
			handle_child_creation(&pip, command, data, envp);
		command = command->next;
	}
	wait_exit(data, pip.i, pip.pid, &pip.command_head);
}
