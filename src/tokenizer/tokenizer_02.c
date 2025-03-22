#include "../../include/minishell.h"

/**
 * @brief Processes the accumulated token value as a TOKEN_WORD.
 *
 * If the provided token_value is non-empty, this function adds it as a TOKEN_WORD
 * token to the tokenizer's token list.
 *
 * @param data Pointer to the minishell data structure.
 * @param tok Double pointer to the tokenizer structure.
 * @param token_value The string value to process.
 * @return t_token* Pointer to the newly added token, or NULL if token_value is empty.
 */
t_token	*process_token_value(t_data *data, t_tokenizer **tok, char *token_value)
{
	if (token_value && token_value[0])
	{
		return (add_token(data, tok, TOKEN_WORD, token_value));
	}
	return (NULL);
}

/**
 * @brief Handles a word token by extracting a substring and expanding variables.
 *
 * This function extracts a substring from the full command based on the current tokenizer index,
 * expands any variables in the substring using expand_variables(), and accumulates the result
 * into the tokenizer's token_value. The final token value is returned.
 *
 * @param tok Double pointer to the tokenizer structure.
 * @param data Pointer to the minishell data structure.
 * @return char* The processed word token value, or NULL on error.
 */
char	*handle_word(t_tokenizer **tok, t_data *data)
{
	char	*tmp;
	char	*tmp2;
	int		start;

	start = (*tok)->i;
	while (should_continue_parsing(tok))
		(*tok)->i++;
	tmp = ft_substr((*tok)->full_cmd, start, (*tok)->i - start);
	tmp2 = expand_variables(tmp, data);
	free(tmp);
	if (!tmp2)
		return (tokenizer_error(data, tok, false), NULL);
	if (!(*tok)->token_value)
		(*tok)->token_value = tmp2;
	else
	{
		(*tok)->token_value = ft_strjoin((*tok)->token_value, tmp2);
		free(tmp2);
		if (!(*tok)->token_value)
			return (tokenizer_error(data, tok, false), NULL);
	}
	return ((*tok)->token_value);
}

/**
 * @brief Handles a quoted token by extracting its content and expanding variables if needed.
 *
 * This function parses a quoted segment from the full command. It extracts the substring between
 * the starting index and the matching quote, and if the quote is not a single quote, expands any
 * variables within the substring. The result is concatenated into the tokenizer's token_value.
 *
 * @param tok Double pointer to the tokenizer structure.
 * @param data Pointer to the minishell data structure.
 * @param start The starting index of the quoted segment.
 * @param tmp An array of two char pointers used for temporary storage.
 * @return char* The updated token_value containing the processed quoted content, or NULL on error.
 */
char	*handle_quotes(t_tokenizer **tok, t_data *data, int start, char **tmp)
{
	while ((*tok)->full_cmd[(*tok)->i]
		&& (*tok)->full_cmd[(*tok)->i] != (*tok)->quote)
		(*tok)->i++;
	if (!(*tok)->full_cmd[(*tok)->i])
		return (tokenizer_error(data, tok, true), NULL);
	tmp[0] = ft_substr((*tok)->full_cmd, start, (*tok)->i - start);
	if ((*tok)->quote != '\'')
	{
		tmp[1] = expand_variables(tmp[0], data);
		free(tmp[0]);
	}
	else
		tmp[1] = tmp[0];
	if (!tmp[1])
		return (tokenizer_error(data, tok, false), NULL);
	if (!(*tok)->token_value)
		(*tok)->token_value = tmp[1];
	else
	{
		(*tok)->token_value = ft_strjoin((*tok)->token_value, tmp[1]);
		free(tmp[1]);
		if (!(*tok)->token_value)
			return (tokenizer_error(data, tok, false), NULL);
	}
	return ((*tok)->token_value);
}

/**
 * @brief Manages quotes within the tokenization process.
 *
 * Checks if the current character in the full command is a quote (and not in a here-document)
 * and, if so, processes the quoted string using handle_quotes(). The tokenizer's quote flag is
 * set and then cleared after processing.
 *
 * @param tok Double pointer to the tokenizer structure.
 * @param full_cmd The full command string being tokenized.
 * @param data Pointer to the minishell data structure.
 * @return true if a quoted segment was successfully processed (OK), false otherwise.
 */
bool	manage_quotes(t_tokenizer **tok, char *full_cmd, t_data *data)
{
	char	*tmp[2];

	if (is_quote(full_cmd[(*tok)->i]) && !(*tok)->in_here_doc)
	{
		(*tok)->quote = full_cmd[(*tok)->i];
		(*tok)->i++;
		(*tok)->token_value = handle_quotes(tok, data, (*tok)->i, tmp);
		(*tok)->i++;
		(*tok)->quote = 0;
		return (OK);
	}
	return (ERROR);
}

/**
 * @brief Tokenizes a segment of the full command string.
 *
 * This function is the inner loop for tokenization. It processes the full command string,
 * handling quoted segments with manage_quotes() and unquoted words with handle_word(). When a delimiter
 * is encountered or the end of the string is reached, it finalizes the current token by calling
 * process_token_value() and returns the token.
 *
 * @param tok Double pointer to the tokenizer structure.
 * @param data Pointer to the minishell data structure.
 * @param current Pointer to the current token pointer.
 * @return t_token* Pointer to the token generated from the current segment.
 */
t_token	*token_inner_loop(t_tokenizer **tok, t_data *data, t_token **current)
{
	while (1 && (*tok)->stop == false)
	{
		if (manage_quotes(tok, (*tok)->full_cmd, data) == OK)
			continue ;
		else if (!is_delimiter((*tok)->full_cmd[(*tok)->i]))
		{
			(*tok)->token_value = handle_word(tok, data);
			(*tok)->in_here_doc = false;
			if ((*tok)->full_cmd[(*tok)->i] == '\0')
			{
				*current = process_token_value(data, tok, (*tok)->token_value);
				(*tok)->token_value = NULL;
				break ;
			}
		}
		else
		{
			(*tok)->in_here_doc = false;
			*current = process_token_value(data, tok, (*tok)->token_value);
			(*tok)->token_value = NULL;
			break ;
		}
	}
	return (*current);
}
