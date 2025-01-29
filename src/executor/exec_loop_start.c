/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_loop_start.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 16:51:20 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/28 17:58:46 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	create_pipe_if_needed(t_pip_vars *pip, t_command *command)
{
	if (command->next)
		if (pipe(pip->pipefd) == -1)
			return (perror("pipe"), exit(EXIT_FAILURE));
}

void	execute_child_process(t_command *command, t_data *data, char **envp,
		t_pip_vars *pip)
{
	pip->command_path = manage_redirs(command, envp, pip, data);
	if ((check_cmd_args(command) && !check_builtin(command, data))
		&& execve(pip->command_path, command->args, envp))
		execve_error_exit(command, pip->command_path);
	exit(g_exit_code);
}

void	handle_parent_process(t_pip_vars *pip, t_command *command)
{
	if (command->eof != NULL)
		waitpid(pip->pid, &g_exit_code, 0);
	father_process(pip, command);
}

void	execute_pipeline(t_command *command, t_data *data, char **envp)
{
	t_pip_vars	pip;
	int			here_doc_pipe[2];

	init_pip(&pip, &command);
	while (command != NULL)
	{
		pip.i++;
		create_pipe_if_needed(&pip, command);
		if (command->eof != NULL)
		{
			handle_here_doc(command, data, here_doc_pipe);
			pip.in_fd = here_doc_pipe[0];
		}
		g_exit_code = 0;
		pip.pid = fork();
		if (pip.pid == -1)
			return (perror("fork"), exit(EXIT_FAILURE));
		if (pip.pid == 0)
			execute_child_process(command, data, envp, &pip);
		else
			handle_parent_process(&pip, command);
		command = command->next;
	}
	wait_exit(pip.i, pip.pid, &pip.command_head);
}
