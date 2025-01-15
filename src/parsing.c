#include "../include/minishell.h"

t_command	*parse_tokens(t_token *tokens)
{
	t_command	*command;
	t_token		*current;
	int			arg_count;

	command = malloc(sizeof(t_command));
	command->args = NULL;
	command->eof = NULL;
	command->input_redirection = NULL;
	command->output_redirection = 0;
	command->append = 0;
	command->isfather = false;
	command->next = NULL;
	current = tokens;
	arg_count = 0;
	while (current && current->type != TOKEN_PIPE)
	{
		if (current->type == TOKEN_HEREDOC)
		{
			command->input_redirection = ft_strdup("heredoc");
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
				command->input_redirection = ft_strdup(current->value);
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
		current = current->next;
	}
	return (command);
}

t_command	*parse_pipeline(t_token *tokens)
{
	t_command	*head;
	t_command	*current_command;
	t_command	*new_command;

	head = NULL;
	current_command = NULL;
	while (tokens)
	{
		new_command = parse_tokens(tokens);
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
