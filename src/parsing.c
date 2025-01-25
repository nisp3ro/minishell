/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 13:21:57 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/24 15:15:06 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_redir	*add_redir(t_redir **redir, t_redir_type type, char *value)
{
	t_redir	*new_redir;
	t_redir	*last;

	new_redir = malloc(sizeof(t_redir));
	last = *redir;
	new_redir->type = type;
	new_redir->value = ft_strdup(value);
	new_redir->next = NULL;
	if (!last)
	{
		*redir = new_redir;
		return (new_redir);
	}
	while (last->next)
		last = last->next;
	last->next = new_redir;
	return (new_redir);
}

t_command	*initialize_command(void)
{
	t_command	*command;

	command = malloc(sizeof(t_command));
	command->args = NULL;
	command->eof = NULL;
	command->append = 0;
	command->redir = NULL;
	command->next = NULL;
	return (command);
}

bool	handle_export_variable(t_token *current, t_data *data,
		t_command *command, int *arg_count)
{
	char	*tmp;

	tmp = ft_strchr(current->value, '=');
	if (tmp && *(tmp + 1) && *(tmp + 1) != ' ' && *(tmp + 1) != '='
		&& current->next == NULL)
		handle_variable_assignment(current->value, &data->vars, data);
	*tmp = '\0';
	command->args = ft_realloc(command->args, sizeof(char *) * (*arg_count
				+ 2));
	command->args[(*arg_count)++] = ft_strdup(current->value);
	command->args[*arg_count] = NULL;
	return (true);
}

bool	handle_heredoc(t_token **current, t_command *command)
{
	*current = (*current)->next;
	if (*current && (*current)->type == TOKEN_WORD)
	{
		command->eof = ft_strdup((*current)->value);
		return (true);
	}
	write(STDERR_FILENO, "syntax error near unexpected token `newline'\n", 45);
	return (false);
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
			add_redir(&command->redir, OUTPUT, (*current)->value);
		else
			add_redir(&command->redir, INPUT, (*current)->value);
		if (type == TOKEN_APPEND_OUT)
			command->append = true;
		else
			command->append = false;
		return (true);
	}
	write(STDERR_FILENO, "syntax error near unexpected token `newline'\n", 45);
	return (false);
}

void	handle_command_args(t_token *current, t_command *command,
		int *arg_count, bool *export)
{
	command->args = ft_realloc(command->args, sizeof(char *) * (*arg_count
				+ 2));
	command->args[(*arg_count)++] = ft_strdup(current->value);
	command->args[*arg_count] = NULL;
	if (!ft_strncmp(current->value, "export", 7))
		*export = true;
}

static bool parse_token_conditions(t_token **current, t_data *data, t_command *command, int *arg_count, bool *export, bool *first)
{
    if (*export && (*current)->type == TOKEN_WORD && ft_strchr((*current)->value, '=') && is_valid_identifier((*current)->value) == OK)
        *export = handle_export_variable(*current, data, command, arg_count);
    else if ((*current)->type == TOKEN_WORD && ft_strchr((*current)->value, '=') && is_valid_identifier((*current)->value) == OK && *first)
        handle_variable_assignment((*current)->value, &data->vars, data);
    else if ((*current)->type == TOKEN_HEREDOC && !handle_heredoc(current, command))
        return false;
    else if ((*current)->type == TOKEN_WORD)
        handle_command_args(*current, command, arg_count, export);
    else if ((*current)->type == TOKEN_REDIRECT_IN && !handle_redirection(current, command, false))
        return false;
    else if (((*current)->type == TOKEN_REDIRECT_OUT || (*current)->type == TOKEN_APPEND_OUT) && !handle_redirection(current, command, true))
        return false;
    return true;
}

t_command *parse_tokens(t_data *data, t_token *tokens)
{
    t_command *command = initialize_command();
    t_token *current = tokens;
    int arg_count = 0;
    bool export = false;
    bool first = true;

    while (current && current->type != TOKEN_PIPE)
    {
        if (!parse_token_conditions(&current, data, command, &arg_count, &export, &first))
            return NULL;
        first = false;
        current = current->next;
    }
    return command;
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
			return (NULL);
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
