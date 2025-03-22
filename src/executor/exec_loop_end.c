#include "../../include/minishell.h"

/**
 * @brief Waits for child processes to exit and updates the global exit code.
 *
 * This function cleans up the command list and then waits for 'i' child processes
 * to finish using waitpid(). It updates the global exit code (data->g_exit_code) based
 * on the exit status of the processes. If a process terminates with status 2 or 3, it
 * adjusts the exit code by adding 128. The final exit code is taken from the process with
 * PID equal to the provided pid. Finally, it calls wait_signal() with argument 1.
 *
 * @param data Pointer to the minishell data structure.
 * @param i The number of child processes to wait for.
 * @param pid The PID of the main child process whose exit code should be retained.
 * @param command Double pointer to the command list to be cleaned.
 */
void	wait_exit(t_data *data, int i, int pid, t_command **command)
{
	int	temp_pid;
	int	temp;

	clean_cmd(*command);
	temp = data->g_exit_code;
	while (i > 0)
	{
		temp_pid = waitpid(-1, &data->g_exit_code, 0);
		if (WIFEXITED(data->g_exit_code))
			data->g_exit_code = WEXITSTATUS(data->g_exit_code);
		if (temp_pid == pid)
			temp = data->g_exit_code;
		if (data->g_exit_code == 2 || data->g_exit_code == 3)
			data->g_exit_code = data->g_exit_code + 128;
		i--;
	}
	data->g_exit_code = temp;
	wait_signal(1);
}

/**
 * @brief Handles file descriptor management in the parent process after forking.
 *
 * In the parent process, this function closes the input file descriptor if it is not
 * standard input. If there is a subsequent command in the pipeline, it closes the write
 * end of the pipe and sets the input file descriptor for the next command to the read end
 * of the current pipe.
 *
 * @param pip Pointer to the structure holding pipe-related variables.
 * @param command Pointer to the current command in the pipeline.
 */
void	father_process(t_pip_vars *pip, t_command *command)
{
	if (pip->in_fd != STDIN_FILENO)
		close(pip->in_fd);
	if (command->next)
	{
		close(pip->pipefd[1]);
		pip->in_fd = pip->pipefd[0];
	}
}

/**
 * @brief Handles errors encountered during execve() and exits appropriately.
 *
 * Depending on the errno set by a failed execve() call, this function writes an
 * appropriate error message to the standard error output and exits with a specific
 * exit code. The exit codes used are:
 * - 126 for permission issues or execution format errors.
 * - 127 for file not found or command not found.
 *
 * @param command Pointer to the command structure whose execution failed.
 * @param command_path The path of the command that was attempted to be executed.
 */
void	execve_error_exit(t_command *command, char *command_path)
{
	if (errno == EACCES)
	{
		if (access(command_path, X_OK) != 0)
			write(STDERR_FILENO, " Permission denied\n", 19);
		else
			write(STDERR_FILENO, " Is a directory\n", 16);
		exit(126);
	}
	else if (errno == ENOEXEC)
		return (write(STDERR_FILENO,
				" Exec format error. Wrong Architecture.\n", 40),
			exit(126));
	else if (errno == EISDIR)
		return (write(STDERR_FILENO, " Is a directory\n", 16), exit(126));
	else if (errno == ENOTDIR)
		return (write(STDERR_FILENO, " Not a directory\n", 17), exit(126));
	else if (errno == ENOENT)
		return (write(STDERR_FILENO, " No such file or directory\n", 27),
			exit(127));
	else
		return (write(STDERR_FILENO, command->args[0],
				ft_strlen(command->args[0])), write(STDERR_FILENO,
				": command not found\n", 20), exit(127));
}
