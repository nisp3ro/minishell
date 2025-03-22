#include "../../include/minishell.h"

/**
 * @brief Checks if the current token meets export conditions.
 *
 * This function determines if the current token should be treated as an export variable.
 * The conditions are that export is enabled, the token is of type TOKEN_WORD,
 * it contains an '=' character, and the token's value is a valid identifier.
 *
 * @param current Double pointer to the current token.
 * @param export Pointer to a boolean flag indicating whether export is enabled.
 * @return true if the token meets the export condition, false otherwise.
 */
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

/**
 * @brief Parses a token as either a redirection or a command argument.
 *
 * This function examines the current token and processes it based on its type.
 * If the token is a word, it calls handle_command_args() to add the token as an argument.
 * If the token is an input redirection, it calls handle_redirection() with false to process it.
 * If the token is an output or append redirection, it calls handle_redirection() with true.
 *
 * @param data Pointer to the minishell data structure.
 * @param current Double pointer to the current token.
 * @param command Pointer to the command structure being built.
 * @return true if the token is parsed successfully, false otherwise.
 */
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

/**
 * @brief Parses token conditions and processes each token accordingly.
 *
 * This function checks various conditions for the current token:
 * - If the token meets export conditions, it processes it as an export variable.
 * - If the token is a TOKEN_WORD containing '=' and is a valid identifier and this is the first token,
 *   it handles it as a variable assignment.
 * - If the token is a here-document, it processes it accordingly.
 * - Otherwise, it parses the token as a redirection or a command.
 *
 * @param current Double pointer to the current token.
 * @param data Pointer to the minishell data structure.
 * @param command Pointer to the command structure being built.
 * @return true if the token is parsed successfully, false otherwise.
 */
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
		write(1, "hola", 4);
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

/**
 * @brief Parses tokens into a command structure until a pipe token is encountered.
 *
 * This function creates a new command by processing tokens until a TOKEN_PIPE is found.
 * It calls parse_token_conditions() for each token, and if an error occurs, it frees the
 * allocated command and returns NULL. It also handles syntax errors for consecutive pipe tokens.
 *
 * @param data Pointer to the minishell data structure.
 * @param tokens Pointer to the first token in the tokens list.
 * @return t_command* Pointer to the newly created command structure, or NULL on error.
 */
t_command	*parse_tokens(t_data *data, t_token *tokens)
{
	t_command	*command;
	t_token		*current;

	current = tokens;
	command = initialize_command();
	if (!command)
		return (NULL);
	while (current && current->type != TOKEN_PIPE)
	{
		if (!parse_token_conditions(&current, data, command))
			return (free(command), NULL);
		command->first = false;
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

/**
 * @brief Parses a sequence of tokens into a pipeline of commands.
 *
 * This function processes the token list and splits it into multiple commands separated by pipe tokens.
 * For each segment, it calls parse_tokens() to generate a command structure. If any command creation
 * fails, it cleans up the commands list and returns NULL.
 *
 * @param data Pointer to the minishell data structure.
 * @param tokens Pointer to the first token in the tokens list.
 * @return t_command* Pointer to the head of the command pipeline, or NULL on error.
 */
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
