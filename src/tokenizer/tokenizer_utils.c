#include "../../include/minishell.h"

/**
 * @brief Determines whether token parsing should continue.
 *
 * This function checks the current character in the full command and returns true if
 * parsing should continue. Parsing continues if the current character is not a delimiter,
 * not a quote (unless in a here-document context), or if it is in a here-document context and
 * not a delimiter.
 *
 * @param tok Double pointer to the tokenizer structure.
 * @return true if parsing should continue, false otherwise.
 */
bool	should_continue_parsing(t_tokenizer **tok)
{
	if ((*tok)->full_cmd[(*tok)->i]
		&& (!is_delimiter((*tok)->full_cmd[(*tok)->i])
			&& !is_quote((*tok)->full_cmd[(*tok)->i]) && !(*tok)->in_here_doc))
		return (true);
	else if ((*tok)->full_cmd[(*tok)->i] && (*tok)->in_here_doc
		&& (!is_delimiter((*tok)->full_cmd[(*tok)->i])))
		return (true);
	else
		return (false);
}

/**
 * @brief Handles a tokenizer error by stopping further tokenization.
 *
 * Sets the stop flag in the tokenizer to true and, if a syntax error is indicated,
 * prints a syntax error message to standard error and sets the global exit code to 2.
 *
 * @param data Pointer to the minishell data structure.
 * @param tok Double pointer to the tokenizer structure.
 * @param syntax_error Boolean flag indicating if the error is a syntax error.
 */
void	tokenizer_error(t_data *data, t_tokenizer **tok, bool syntax_error)
{
	(*tok)->stop = true;
	if (syntax_error)
	{
		write(2, "minishell: syntax error\n", 24);
		data->g_exit_code = 2;
	}
}
