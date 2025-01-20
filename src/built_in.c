/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 13:24:46 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/20 13:35:15 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

bool	check_builtin(t_command *command, t_data *data)
{
	if (!command->args)
		return (false);
	if (ft_strncmp(command->args[0], "exit", 5) == 0)
		return (ft_exit(data, command), true);
	if (ft_strncmp(command->args[0], "echo", 5) == 0)
		return (ft_echo(command), true);
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
	return (false);
}

bool	check_redirs(t_command *command)
{
	bool	input_redirection;
	bool	output_redirection;
	int		fd_in;
	int		fd_out;

	input_redirection = false;
	output_redirection = false;
	fd_in = STDIN_FILENO;
	fd_out = STDOUT_FILENO;
	while (command->redir)
	{
		if (command->redir->type == INPUT)
		{
			input_redirection = true;
			if (fd_in != STDIN_FILENO)
				close(fd_in);
			fd_in = open(command->redir->value, O_RDONLY);
			if (fd_in < 0)
			{
				perror("open");
				return (false);
			}
		}
		else if (command->redir->type == OUTPUT)
		{
			output_redirection = true;
			if (fd_out != STDOUT_FILENO)
				close(fd_out);
			if (command->append)
				fd_out = open(command->redir->value,
								O_WRONLY | O_CREAT | O_APPEND,
								0644);
			else
				fd_out = open(command->redir->value,
								O_WRONLY | O_CREAT | O_TRUNC,
								0644);
			if (fd_out < 0)
			{
				perror("open");
				return (false);
			}
		}
		command->redir = command->redir->next;
	}
	if (input_redirection)
	{
		dup2(fd_in, STDIN_FILENO);
		close(fd_in);
	}
	if (output_redirection)
	{
		dup2(fd_out, STDOUT_FILENO);
		close(fd_out);
	}
	return (true);
}

bool	check_builtin_prepipe(t_command *command, t_data *data)
{
	if (!command->args)
		return (false);
	if (ft_strncmp(command->args[0], "exit", 5) == 0)
	{
		if (check_redirs(command) == true)
			return (ft_exit(data, command), true);
		return (true);
		//devuelve true aqui para que no entre en ejecutor porque hay fallo en las redirecciones
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
