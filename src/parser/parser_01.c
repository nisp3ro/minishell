/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_01.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 12:38:07 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/30 13:23:27 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static bool	export_condition(t_token **current, bool *export)
{
	if (*export && (*current)->type == TOKEN_WORD
		&& ft_strchr((*current)->value, '=')
		&& is_valid_identifier((*current)->value) == OK)
	{
		return (true);
	}
	return (false);
}

static bool	parse_redirection_or_command(t_data *data, t_token **current,
		t_command *command)
{
	if ((*current)->type == TOKEN_WORD)
	{
		if (!handle_command_args(*current, command))
			return (false);
	}
	else if ((*current)->type == TOKEN_REDIRECT_IN)
	{
		if (!handle_redirection(data, current, command, false))
			return (false);
	}
	else if ((*current)->type == TOKEN_REDIRECT_OUT
		|| (*current)->type == TOKEN_APPEND_OUT)
	{
		if (!handle_redirection(data, current, command, true))
			return (false);
	}
	return (true);
}

static bool	parse_token_conditions(t_token **current, t_data *data,
		t_command *command)
{
	if (export_condition(current, &command->export))
	{
		if (!handle_export_variable(*current, data, command,
				&command->arg_count))
			return (false);
	}
	else if ((*current)->type == TOKEN_WORD && ft_strchr((*current)->value, '=')
		&& is_valid_identifier((*current)->value) == OK && command->first)
	{
		if (!handle_variable_assignment((*current)->value, &data->vars, data))
			return (false);
	}
	else if ((*current)->type == TOKEN_HEREDOC)
	{
		if (!handle_heredoc(data, current, command))
			return (false);
	}
	else if (!parse_redirection_or_command(data, current, command))
	{
		return (false);
	}
	return (true);
}

t_command	*parse_tokens(t_data *data, t_token *tokens)
{
	t_command	*command;
	t_token		*current;
	bool		first;

	current = tokens;
	command = initialize_command();
	if (!command)
		return (NULL);
	while (current && current->type != TOKEN_PIPE)
	{
		if (!parse_token_conditions(&current, data, command))
			return (free(command), NULL);
		first = false;
		current = current->next;
	}
	if (current && current->type == TOKEN_PIPE && current->next
		&& current->next->type == TOKEN_PIPE)
	{
		write(STDERR_FILENO, "syntax error near unexpected token `||'\n", 41);
		data->g_exit_code = 2;
		return (NULL);
	}
	return (command);
}

t_command	*parse_pipeline(t_data *data, t_token *tokens)
{
	t_command	*head;
	t_command	*current_command;
	t_command	*new_command;

	head = NULL;
	current_command = NULL;
	while (tokens)
	{
		new_command = parse_tokens(data, tokens);
		if (new_command == NULL)
			return (clean_cmd(head), NULL);
		else
		{
			if (!head)
				head = new_command;
			else
				current_command->next = new_command;
			current_command = new_command;
		}
		while (tokens && tokens->type != TOKEN_PIPE)
			tokens = tokens->next;
		if (tokens && tokens->type == TOKEN_PIPE)
			tokens = tokens->next;
	}
	return (head);
}
