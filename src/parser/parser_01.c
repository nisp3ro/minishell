/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_01.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 12:38:07 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/25 12:45:51 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static bool export_condition(t_token **current, bool *export) {
    if (*export && (*current)->type == TOKEN_WORD
        && ft_strchr((*current)->value, '=')
        && is_valid_identifier((*current)->value) == OK) {
        return true;
    }
    return false;
}

static bool	parse_token_conditions(t_token **current, t_data *data,
		t_command *command, int *arg_count, bool *export, bool *first)
{
	if (export_condition(current, export))
	{
		if (!handle_export_variable(*current, data, command, arg_count))
			return (false);
	}
	else if ((*current)->type == TOKEN_WORD && ft_strchr((*current)->value, '=')
			&& is_valid_identifier((*current)->value) == OK && *first)
	{
		if (!handle_variable_assignment((*current)->value, &data->vars, data))
			return (false);
	}
	else if ((*current)->type == TOKEN_HEREDOC && !handle_heredoc(current,
				command))
		return (false);
	else if ((*current)->type == TOKEN_WORD && !handle_command_args(*current,
				command, arg_count, export))
		return (false);
	else if ((*current)->type == TOKEN_REDIRECT_IN
			&& !handle_redirection(current, command, false))
		return (false);
	else if (((*current)->type == TOKEN_REDIRECT_OUT
				|| (*current)->type == TOKEN_APPEND_OUT)
			&& !handle_redirection(current, command, true))
		return (false);
	return (true);
}

t_command	*initialize_command(void)
{
	t_command	*command;

	command = malloc(sizeof(t_command));
	if (!command)
		return (NULL);
	command->args = NULL;
	command->eof = NULL;
	command->append = 0;
	command->redir = NULL;
	command->next = NULL;
	return (command);
}

t_command	*parse_tokens(t_data *data, t_token *tokens)
{
	t_command	*command;
	t_token		*current;
	int			arg_count;
	bool		export;
	bool		first;

	current = tokens;
	arg_count = 0;
	export = false;
	first = true;
	command = initialize_command();
	if (!command)
		return (NULL);
	while (current && current->type != TOKEN_PIPE)
	{
		if (!parse_token_conditions(&current, data, command, &arg_count,
				&export, &first))
			return (free(command), NULL);
		first = false;
		current = current->next;
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
