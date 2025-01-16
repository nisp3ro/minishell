#include "../include/minishell.h"

t_command	*parse_tokens(t_data *data, t_token *tokens)
{
	t_command	*command;
	t_token		*current;
	char		*tmp;
	int			arg_count;
	bool		first;
	bool		export;

	command = malloc(sizeof(t_command));
	command->args = NULL;
	command->eof = NULL;
	command->input_redirection = 0;
	command->output_redirection = 0;
	command->append = 0;
	command->next = NULL;
	current = tokens;
	arg_count = 0;
	first = true;
	export = false;
	while (current && current->type != TOKEN_PIPE)
	{
		if (first && current->type == TOKEN_WORD && ft_strchr(current->value, '='))
		{
			tmp = ft_strchr(current->value, '=');
			if (tmp && *(tmp + 1) && *(tmp + 1) != ' ' && *(tmp + 1) != '=' && is_valid_identifier(current->value) == OK)
				handle_variable_assignment(current->value, &data->vars, data);
		}
		else if (export && current->type == TOKEN_WORD && ft_strchr(current->value, '=') && is_valid_identifier(current->value) == OK)
		{
			tmp = ft_strchr(current->value, '=');
			if (tmp && *(tmp + 1) && *(tmp + 1) != ' ' && *(tmp + 1) != '=')
			{
				handle_variable_assignment(current->value, &data->vars, data);
				*tmp = '\0';
				command->args = realloc(command->args, sizeof(char *) * (arg_count + 2)); //sustituir propio
				command->args[arg_count++] = ft_strdup(current->value);
				command->args[arg_count] = NULL;
			}
		}
		else if (current->type == TOKEN_HEREDOC)
		{
			current = current->next;
			if (current && current->type == TOKEN_WORD)
				command->eof= ft_strdup(current->value);
			else
				return(NULL); // Manejar error de sintaxis
		}
		else if (current->type == TOKEN_WORD)
		{
			command->args = realloc(command->args, sizeof(char *) * (arg_count + 2)); //sustituir propio
			command->args[arg_count++] = ft_strdup(current->value);
			command->args[arg_count] = NULL;
		}
		else if (current->type == TOKEN_REDIRECT_IN)
		{
			current = current->next;
			if (current && current->type == TOKEN_WORD)
			{
				command->input_redirection = open(current->value, O_RDONLY);
				if (command->input_redirection < 0)
					{
						perror(" ");
						g_error = 1;
						return(NULL);
					}
			}
			else
				return (NULL); // Manejar error de sintaxis
		}
		else if (current->type == TOKEN_REDIRECT_OUT || current->type == TOKEN_APPEND_OUT)
		{
			command->append = (current->type == TOKEN_APPEND_OUT);
			current = current->next;
			if (current && current->type == TOKEN_WORD && command->append)
				command->output_redirection = open(current->value,
							O_WRONLY | O_CREAT | O_APPEND, 0644);
			else if (current && current->type == TOKEN_WORD && !command->append)
				command->output_redirection = open(current->value,
							O_WRONLY | O_CREAT | O_TRUNC, 0644);
			else
				return (NULL); // Manejar error de sintaxis
		}
		first = false;
		export = (current->type == TOKEN_WORD && !ft_strncmp(current->value, "export", 7));
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
		if (!head)
			head = new_command;
		else
			current_command->next = new_command;
		current_command = new_command;
		while (tokens && tokens->type != TOKEN_PIPE)
			tokens = tokens->next;
		if (tokens && tokens->type == TOKEN_PIPE)
			tokens = tokens->next;
	}
	return (head);
}
