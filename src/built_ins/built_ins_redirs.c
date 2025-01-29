/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins_redirs.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 09:54:19 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/27 17:48:43 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static bool	handle_input_redirection(t_redir *redir, int *fd_in)
{
	if (*fd_in != STDIN_FILENO)
		close(*fd_in);
	*fd_in = open(redir->value, O_RDONLY);
	if (*fd_in < 0)
	{
		perror("open");
		return (false);
	}
	return (true);
}

static bool	handle_output_redirection(t_redir *redir, int *fd_out, bool append)
{
	if (*fd_out != STDOUT_FILENO)
		close(*fd_out);
	if (append)
		*fd_out = open(redir->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		*fd_out = open(redir->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (*fd_out < 0)
	{
		perror("open");
		return (false);
	}
	return (true);
}

static bool	process_redirections(t_command *command, int *fd_in_out,
		bool *i_redir, bool *o_redir)
{
	while (command->redir)
	{
		if (command->redir->type == INPUT)
		{
			*i_redir = true;
			if (!handle_input_redirection(command->redir, &fd_in_out[0]))
				return (false);
		}
		else if (command->redir->type == OUTPUT)
		{
			*o_redir = true;
			if (!handle_output_redirection(command->redir, &fd_in_out[1],
					command->append))
				return (false);
		}
		command->redir = command->redir->next;
	}
	return (true);
}

bool	check_redirs(t_command *command)
{
	int		fd_in_out[2];
	bool	input_redirection;
	bool	output_redirection;

	fd_in_out[0] = STDIN_FILENO;
	fd_in_out[1] = STDOUT_FILENO;
	input_redirection = false;
	output_redirection = false;
	if (!process_redirections(command, fd_in_out, &input_redirection,
			&output_redirection))
		return (false);
	if (input_redirection)
	{
		dup2(fd_in_out[0], STDIN_FILENO);
		close(fd_in_out[0]);
	}
	if (output_redirection)
	{
		dup2(fd_in_out[1], STDOUT_FILENO);
		close(fd_in_out[1]);
	}
	return (true);
}
