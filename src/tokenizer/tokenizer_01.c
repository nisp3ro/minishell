#include "../../include/minishell.h"

/**
 * @brief Adds a new token to the token list.
 *
 * Allocates a new t_token node, sets its type and value, and appends it to the end of the
 * tokenizer's token list. If memory allocation fails, it calls tokenizer_error() and returns NULL.
 *
 * @param data Pointer to the minishell data structure.
 * @param tok Double pointer to the tokenizer structure.
 * @param type The type of the token to add.
 * @param value The value of the token.
 * @return t_token* Pointer to the newly created token, or NULL on error.
 */
t_token	*add_token(t_data *data, t_tokenizer **tok, t_token_type type,
		char *value)
{
	t_token	*new_token;
	t_token	*last;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (tokenizer_error(data, tok, false), NULL);
	last = (*tok)->tokens;
	new_token->type = type;
	new_token->value = value;
	if (!new_token->value)
		return (free(new_token), tokenizer_error(data, tok, false), NULL);
	new_token->next = NULL;
	if (!last)
	{
		(*tok)->tokens = new_token;
		return (new_token);
	}
	while (last->next)
		last = last->next;
	last->next = new_token;
	return (new_token);
}

/**
 * @brief Creates a token for redirection or here-document.
 *
 * Depending on the token type provided, sets the token value to the corresponding symbol.
 * It then calls add_token() to create and append the token to the tokenizer's token list.
 *
 * @param data Pointer to the minishell data structure.
 * @param tok Double pointer to the tokenizer structure.
 * @param type The type of the token to create.
 * @param hd_val Boolean flag indicating if the token is associated with a here-document.
 * @return t_token* Pointer to the created token.
 */
t_token	*check_hd_and_create_tok(t_data *data, t_tokenizer **tok,
		enum e_token_type type, bool hd_val)
{
	t_token	*current;
	char	*value;

	value = NULL;
	(*tok)->in_here_doc = hd_val;
	if (type == TOKEN_PIPE)
		value = "|";
	else if (type == TOKEN_HEREDOC)
		value = "<<";
	else if (type == TOKEN_REDIRECT_IN)
		value = "<";
	else if (type == TOKEN_APPEND_OUT)
		value = ">>";
	else if (type == TOKEN_REDIRECT_OUT)
		value = ">";
	current = add_token(data, tok, type, value);
	return (current);
}

/**
 * @brief Handles a delimiter token and creates the corresponding token.
 *
 * Checks the character at the current tokenizer index in the full command and creates a token
 * based on the delimiter encountered (pipe, here-document, redirection in, append out, or redirection out).
 * Increments the tokenizer index appropriately.
 *
 * @param data Pointer to the minishell data structure.
 * @param tok Double pointer to the tokenizer structure.
 * @return t_token* Pointer to the created token.
 */
t_token	*handle_delimiter(t_data *data, t_tokenizer **tok)
{
	t_token	*cur;

	cur = NULL;
	if ((*tok)->full_cmd[(*tok)->i] == '|')
		cur = check_hd_and_create_tok(data, tok, TOKEN_PIPE, false);
	else if ((*tok)->full_cmd[(*tok)->i] == '<' && (*tok)->full_cmd[(*tok)->i
			+ 1] == '<')
	{
		cur = check_hd_and_create_tok(data, tok, TOKEN_HEREDOC, true);
		(*tok)->i++;
	}
	else if ((*tok)->full_cmd[(*tok)->i] == '<')
		cur = check_hd_and_create_tok(data, tok, TOKEN_REDIRECT_IN, false);
	else if ((*tok)->full_cmd[(*tok)->i] == '>' && (*tok)->full_cmd[(*tok)->i
			+ 1] == '>')
	{
		cur = check_hd_and_create_tok(data, tok, TOKEN_APPEND_OUT, false);
		(*tok)->i++;
	}
	else if ((*tok)->full_cmd[(*tok)->i] == '>')
		cur = check_hd_and_create_tok(data, tok, TOKEN_REDIRECT_OUT, false);
	(*tok)->i++;
	return (cur);
}

/**
 * @brief Initializes the tokenizer structure.
 *
 * Sets the tokenizer's index, quote flag, full command string, token value, here-document flag,
 * token list, and stop flag to their initial values.
 *
 * @param tokenizer Double pointer to the tokenizer structure to initialize.
 * @param current Pointer to the current token (set to NULL).
 * @param full_cmd The full command string to be tokenized.
 */
void	init_tokenizer(t_tokenizer **tokenizer,
					t_token **current,
					char *full_cmd)
{
	t_token	*tokens;

	tokens = NULL;
	(*tokenizer)->i = 0;
	(*tokenizer)->quote = 0;
	(*tokenizer)->full_cmd = full_cmd;
	(*tokenizer)->token_value = NULL;
	(*tokenizer)->in_here_doc = false;
	(*tokenizer)->tokens = tokens;
	(*tokenizer)->stop = false;
	*current = NULL;
}

/**
 * @brief Tokenizes a full command string into a list of tokens.
 *
 * Allocates a tokenizer structure, initializes it, and iterates through the full command string,
 * calling handle_delimiter() for delimiter characters and token_inner_loop() for other tokens.
 * If an error occurs during tokenization (indicated by the stop flag), it frees the tokens and returns NULL.
 *
 * @param full_cmd The full command string to tokenize.
 * @param data Pointer to the minishell data structure.
 * @return t_token* Pointer to the head of the generated token list, or NULL on error.
 */
t_token	*tokenize(char *full_cmd, t_data *data)
{
	t_token		*current;
	t_tokenizer	*tok;
	t_token		*tokens;

	tok = malloc(sizeof(t_tokenizer));
	if (!tok)
		return (NULL);
	init_tokenizer(&tok, &current, full_cmd);
	while (full_cmd[tok->i])
	{
		if (is_delimiter(full_cmd[tok->i]) && !tok->quote)
			current = handle_delimiter(data, &tok);
		else
			current = token_inner_loop(&tok, data, &current);
		if (tok->stop == true)
			return (free_tokens(tok->tokens), NULL);
	}
	tokens = tok->tokens;
	free(tok);
	return (tokens);
}
