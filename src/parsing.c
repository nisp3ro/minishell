/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 13:21:57 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/20 18:59:04 by mrubal-c         ###   ########.fr       */
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

t_command	*parse_tokens(t_data *data, t_token *tokens)
{
	t_command	*command;
	t_token		*current;
	char		*tmp;
	int			arg_count;
	int			o_redir_count;
	int			i_redir_count;
	bool		export;

	command = malloc(sizeof(t_command));
	command->args = NULL;
	command->eof = NULL;
	command->append = 0;
	command->redir = NULL;
	command->next = NULL;
	current = tokens;
	arg_count = 0;
	o_redir_count = 0;
	i_redir_count = 0;
	export = false;
	while (current && current->type != TOKEN_PIPE)
	{
		if (export && current->type == TOKEN_WORD
				&& ft_strchr(current->value, '=')
				&& is_valid_identifier(current->value) == OK)
		{
			tmp = ft_strchr(current->value, '=');
			if (tmp && *(tmp + 1) && *(tmp + 1) != ' ' && *(tmp + 1) != '=')
				handle_variable_assignment(current->value, &data->vars, data);
			*tmp = '\0';
			command->args = ft_realloc(command->args, sizeof(char *)
					* (arg_count + 2)); //sustituir propio
			command->args[arg_count++] = ft_strdup(current->value);
			command->args[arg_count] = NULL;
			export = true;
		}
		else if (current->type == TOKEN_WORD && ft_strchr(current->value,
				'=') && is_valid_identifier(current->value) == OK)
		{
			tmp = ft_strchr(current->value, '=');
			if (tmp && *(tmp + 1) && *(tmp + 1) != ' ' && *(tmp + 1) != '='
				&& is_valid_identifier(current->value) == OK)
				handle_variable_assignment(current->value, &data->vars, data);
		}
		else if (current->type == TOKEN_HEREDOC)
		{
			current = current->next;
			if (current && current->type == TOKEN_WORD)
				command->eof = ft_strdup(current->value);
			else
			{
				write(STDERR_FILENO,
						"syntax error near unexpected token `newline'\n",
						45);
				return (NULL);
			}
			export = false;
		}
		else if (current->type == TOKEN_WORD)
		{
			command->args = ft_realloc(command->args, sizeof(char *)
					* (arg_count + 2)); //sustituir propio
			command->args[arg_count++] = ft_strdup(current->value);
			command->args[arg_count] = NULL;
			if (!ft_strncmp(current->value, "export", 7))
			export = true;
		}
		else if (current->type == TOKEN_REDIRECT_IN)
		{
			current = current->next;
			if (current && current->type == TOKEN_WORD)
				add_redir(&command->redir, INPUT, current->value);
			else
			{
				write(STDERR_FILENO,
						"syntax error near unexpected token `newline'\n",
						45);
				return (NULL);
			}
			export = false;
		}
		else if (current->type == TOKEN_REDIRECT_OUT
				|| current->type == TOKEN_APPEND_OUT)
		{
			command->append = (current->type == TOKEN_APPEND_OUT);
			current = current->next;
			if (current && current->type == TOKEN_WORD)
				add_redir(&command->redir, OUTPUT, current->value);
			else
			{
				write(STDERR_FILENO,
						"syntax error near unexpected token `newline'\n",
						45);
				return (NULL);
			}
			export = false;
		}
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
		if (new_command->args == NULL || new_command->args[0] == NULL
			|| new_command->args[0][0] == '\0')
		{
			new_command = NULL;
			//free_command(new_command);
		}
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
