/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_02.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 12:37:57 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/27 13:05:37 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	add_redir(t_redir **redir, t_redir_type type, char *value)
{
	t_redir	*new_redir;
	t_redir	*last;

	new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		return (false);
	last = *redir;
	new_redir->type = type;
	new_redir->value = ft_strdup(value);
	if (!new_redir->value)
		return (free(new_redir), false);
	new_redir->next = NULL;
	if (!last)
	{
		*redir = new_redir;
		return (true);
	}
	while (last->next)
		last = last->next;
	last->next = new_redir;
	return (true);
}

bool	handle_redirection(t_token **current, t_command *command,
		bool is_output)
{
	t_token_type	type;

	type = (*current)->type;
	*current = (*current)->next;
	if (*current && (*current)->type == TOKEN_WORD)
	{
		if (is_output)
		{
			if (!add_redir(&command->redir, OUTPUT, (*current)->value))
				return (clean_redir_list(&command->redir), false);
		}
		else
		{
			if (!add_redir(&command->redir, INPUT, (*current)->value))
				return (clean_redir_list(&command->redir), false);
		}
		if (type == TOKEN_APPEND_OUT)
			command->append = true;
		else
			command->append = (false);
		return (true);
	}
	write(STDERR_FILENO, "syntax error near unexpected token `newline'\n", 45);
	g_exit_code = 2;
	return (false);
}

bool	handle_command_args(t_token *current, t_command *command)
{
	command->args = ft_realloc(command->args, sizeof(char *)
			* (command->arg_count + 2));
	if (!command->args)
		return (false);
	command->args[(command->arg_count)++] = ft_strdup(current->value);
	if (!command->args[command->arg_count - 1])
		return (clean_mtx(command->args), false);
	command->args[command->arg_count] = NULL;
	if (!ft_strncmp(current->value, "export", 7))
		command->export = true;
	return (true);
}

bool	handle_heredoc(t_token **current, t_command *command)
{
	*current = (*current)->next;
	if (*current && (*current)->type == TOKEN_WORD)
	{
		command->eof = ft_realloc(command->eof, sizeof(char *)
				* (command->eof_count + 2));
		if (!command->eof)
			return (false);
		command->eof[command->eof_count++] = ft_strdup((*current)->value);
		if (!command->eof[command->eof_count - 1])
			return (clean_mtx(command->eof), false);
		command->eof[command->eof_count] = NULL;
		return (true);
	}
	write(STDERR_FILENO, "syntax error near unexpected token `newline'\n", 45);
	g_exit_code = 2;
	return (false);
}

bool	handle_export_variable(t_token *current, t_data *data,
		t_command *command, int *arg_count)
{
	char	*tmp;

	tmp = ft_strchr(current->value, '=');
	if (tmp && *(tmp + 1) != ' ' && *(tmp + 1) != '=')
	{
		handle_variable_assignment(current->value, &data->vars, data);
		*tmp = '\0';
	}
	command->args = ft_realloc(command->args, sizeof(char *) * (*arg_count
				+ 2));
	if (!command->args)
		return (false);
	command->args[(*arg_count)++] = ft_strdup(current->value);
	if (!command->args[*arg_count - 1])
		return (clean_mtx(command->args), false);
	command->args[*arg_count] = NULL;
	return (true);
}
