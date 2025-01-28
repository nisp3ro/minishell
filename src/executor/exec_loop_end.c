/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_loop_end.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 13:40:12 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/28 17:59:50 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	wait_exit(int i, int pid, t_command **command)
{
	int	temp_pid;
	int	temp;

	clean_cmd(*command);
	while (i >= 0)
	{
		temp_pid = waitpid(-1, &g_exit_code, 0);
		if (WIFEXITED(g_exit_code))
			g_exit_code = WEXITSTATUS(g_exit_code);
		if (temp_pid == pid)
			temp = g_exit_code;
		if (g_exit_code == 2 || g_exit_code == 3)
			g_exit_code = g_exit_code + 128;
		i--;
	}
	g_exit_code = temp;
	wait_signal(1);
}

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
