/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 17:18:36 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/28 17:20:17 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	redir(t_command *command, t_redir_vars *red)
{
	if (command->redir->type == INPUT)
	{
		red->input_redirection = true;
		if (red->fd_in != STDIN_FILENO)
			close(red->fd_in);
		red->fd_in = open(command->redir->value, O_RDONLY);
		if (red->fd_in < 0)
			return (perror("open"), exit(EXIT_FAILURE));
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
			return (perror("open"), exit(EXIT_FAILURE));
	}
	command->redir = command->redir->next;
}
//OJO LIMPIAR EN LOS RETRUN?

void	init_redir_vars(t_redir_vars *red)
{
	red->fd_in = STDIN_FILENO;
	red->fd_out = STDOUT_FILENO;
	red->input_redirection = false;
	red->output_redirection = false;
}

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

char	*manage_redirs(t_command *command, char **envp, t_pip_vars *pip,
		t_data *data)
{
	t_redir_vars	red;
	int				eof_i;
	int				here_doc_pipe[2];

	init_redir_vars(&red);
	if (command->args && ft_strchr(command->args[0], '/') != 0)
		ft_create_custom_path(&red.command_path, command);
	else if (command->args)
		red.command_path = find_command_in_path(command->args[0], envp);
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
		close(pip->pipefd[0]);
	}
	manage_redirections(&red);
	return (red.command_path);
}
