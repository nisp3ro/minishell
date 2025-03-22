#include "../../include/minishell.h"

/**
 * @brief Adds a redirection to the redirection list.
 *
 * Allocates a new t_redir node, sets its type and value (duplicated from the provided value),
 * and appends it to the end of the redirection list.
 *
 * @param redir Pointer to the head of the redirection list.
 * @param type The type of redirection (INPUT, OUTPUT, etc.).
 * @param value The file path associated with the redirection.
 * @return true if the redirection was added successfully, false otherwise.
 */
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

/**
 * @brief Handles a redirection token.
 *
 * Processes a redirection token from the token stream by consuming the next token
 * (which should be a file name) and adding the appropriate redirection to the command.
 * It distinguishes between input and output redirections, and sets the command's append flag
 * if needed. On a syntax error (missing file name), an error message is printed, the global
 * exit code is set, and false is returned.
 *
 * @param data Pointer to the minishell data structure.
 * @param current Double pointer to the current token.
 * @param command Pointer to the command being built.
 * @param is_output Boolean indicating if the redirection is for output.
 * @return true if the redirection is handled successfully, false otherwise.
 */
bool	handle_redirection(t_data *data, t_token **current, t_command *command,
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
	data->g_exit_code = 2;
	return (false);
}

/**
 * @brief Handles a command argument token.
 *
 * Reallocates the command's argument array to add the new argument, duplicates the token's value,
 * and updates the argument count. If the token's value is "export", the command's export flag is set.
 *
 * @param current Pointer to the current token.
 * @param command Pointer to the command being built.
 * @return true if the argument is handled successfully, false otherwise.
 */
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

/**
 * @brief Handles a heredoc token.
 *
 * Consumes the heredoc token by moving to the next token (expected to be the limiter),
 * reallocates the command's EOF array to include the limiter, duplicates its value,
 * and increments the EOF count. On a syntax error (missing limiter), an error message is printed,
 * the global exit code is set, and false is returned.
 *
 * @param data Pointer to the minishell data structure.
 * @param current Double pointer to the current token.
 * @param command Pointer to the command being built.
 * @return true if the heredoc is handled successfully, false otherwise.
 */
bool	handle_heredoc(t_data *data, t_token **current, t_command *command)
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
	data->g_exit_code = 2;
	return (false);
}

/**
 * @brief Handles an export variable token.
 *
 * Processes a token representing an export variable by checking for an '=' character.
 * If the token contains an '=' and the character following it is not a space or another '=',
 * a variable assignment is handled. The token's value is then truncated at the '=' character
 * and added as a command argument. The argument count is updated accordingly.
 *
 * @param current Pointer to the current token.
 * @param data Pointer to the minishell data structure.
 * @param command Pointer to the command being built.
 * @param arg_count Pointer to the current argument count.
 * @return true if the export variable is handled successfully, false otherwise.
 */
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
