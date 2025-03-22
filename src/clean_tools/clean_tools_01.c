#include "../../include/minishell.h"

/**
 * @brief Frees a linked list of tokens.
 *
 * Iterates through the token list and frees each token node. If a token's type is
 * TOKEN_WORD, its value is freed before freeing the token itself.
 *
 * @param tokens Pointer to the first token in the linked list.
 */
void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		if (tmp->type == TOKEN_WORD)
			free(tmp->value);
		free(tmp);
	}
}

/**
 * @brief Frees a linked list of variables.
 *
 * Iterates through the variable list and frees the memory allocated for each
 * variable's name, value, and the node itself.
 *
 * @param vars Pointer to the first variable in the linked list.
 */
void	clean_variables(t_vars *vars)
{
	t_vars	*tmp;

	while (vars)
	{
		tmp = vars;
		vars = vars->next;
		free(tmp->name);
		free(tmp->value);
		free(tmp);
	}
}

/**
 * @brief Frees a null-terminated matrix of strings.
 *
 * Iterates through the matrix and frees each string. After freeing all strings,
 * the matrix itself is freed.
 *
 * @param mtx Pointer to the matrix (array of strings) to be freed.
 */
void	clean_mtx(char **mtx)
{
	int	i;

	i = 0;
	while (mtx && mtx[i])
	{
		free(mtx[i]);
		i++;
	}
	if (mtx)
		free(mtx);
}

/**
 * @brief Cleans a line by writing it to the history and trimming spaces.
 *
 * If the line is not empty and is not composed solely of spaces, it writes the line
 * to the history. Then, it trims leading and trailing spaces from the line, frees
 * the original line, and returns the trimmed version.
 *
 * @param line The input line to be cleaned.
 * @param data Pointer to the minishell data structure.
 * @return char* Pointer to the cleaned (trimmed) line.
 */
char	*clean_line(char *line, t_data *data)
{
	char	*full_cmd;

	if (line[0] != '\0' && !is_all_spaces(line))
		ft_write_history(data, line);
	full_cmd = ft_strtrim(line, " ");
	free(line);
	return (full_cmd);
}
