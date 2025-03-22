#include "../../include/minishell.h"

/**
 * @brief Advances the index to determine the length of a variable name.
 *
 * Checks if the character at position *j in token_value is alphabetic.
 * If so, it continues advancing *j while characters are alphanumeric or underscores.
 * Otherwise, it advances *j by one.
 *
 * @param token_value The token string containing the variable.
 * @param j Pointer to the index to be advanced.
 */
static void	var_name_len(char *token_value, int *j)
{
	if (ft_isalpha(token_value[*j]))
	{
		while ((token_value[*j] != '\0' && ft_isalnum(token_value[*j]))
			|| (token_value[*j] != '\0' && token_value[*j] == '_'))
			(*j)++;
	}
	else
		(*j)++;
}

/**
 * @brief Handles expansion of a variable from a token string.
 *
 * Starting from index *i in token_value (which should point to a '$'),
 * this function extracts the variable name, retrieves its value from the environment
 * or internal variables, and appends that value to the accumulated string in *expanded.
 *
 * @param token_value The token string containing the variable.
 * @param i Pointer to the current index in token_value; it will be advanced past the variable name.
 * @param expanded Pointer to the accumulated expanded string.
 * @param data Pointer to the minishell data structure.
 * @return char* The updated expanded string, or NULL on error.
 */
static char	*handle_variable_expansion(char *token_value, int *i,
		char **expanded, t_data *data)
{
	char	*var_val[2];
	char	*tmp;
	int		j;
	int		k;

	j = *i + 1;
	k = 0;
	var_name_len(token_value, &j);
	var_val[0] = malloc(sizeof(char) * (j - *i + 1));
	if (!var_val[0])
		return (free(*expanded), NULL);
	while (*i < j)
		var_val[0][k++] = token_value[(*i)++];
	var_val[0][k] = '\0';
	var_val[1] = mini_getenv(var_val[0] + 1, data->envp);
	if (!var_val[1])
		var_val[1] = mini_getvars(data->vars, var_val[0] + 1);
	if (!var_val[1])
		var_val[1] = "";
	tmp = ft_strjoin(*expanded, var_val[1]);
	free(*expanded);
	*expanded = tmp;
	return (free(var_val[0]), *expanded);
}

/**
 * @brief Processes a single character or variable expansion in a token.
 *
 * Examines the current character in token_value (at index *i). If the character
 * is '$' followed by '?', it handles exit code expansion; if it is '$' followed
 * by a non-null character, it handles variable expansion; otherwise, it appends
 * the literal character to *expanded.
 *
 * @param token_value The token string being processed.
 * @param i Pointer to the current index in token_value.
 * @param expanded Pointer to the accumulated expanded string.
 * @param data Pointer to the minishell data structure.
 * @return int Returns OK on success or ERROR on failure.
 */
static int	process_token_character(char *token_value, int *i, char **expanded,
		t_data *data)
{
	if (token_value[*i] == '$' && token_value[*i + 1] == '?')
	{
		if (!handle_exit_code_expansion(data, i, expanded))
			return (ERROR);
	}
	else if (token_value[*i] == '$' && token_value[*i + 1] != '\0')
	{
		if (!handle_variable_expansion(token_value, i, expanded, data))
			return (ERROR);
	}
	else
	{
		if (!append_literal_char(token_value[(*i)++], expanded))
			return (ERROR);
	}
	return (OK);
}

/**
 * @brief Updates quote status based on the current character.
 *
 * Checks the character at index i in token_value. If it is a single quote,
 * toggles the single quote flag; if it is a double quote, toggles the double quote flag.
 *
 * @param token_value The token string.
 * @param i The current index in the token string.
 * @param s_quote Pointer to a boolean flag indicating if a single quote is active.
 * @param d_quote Pointer to a boolean flag indicating if a double quote is active.
 */
void	check_quotes(char *token_value, int i, bool *s_quote, bool *d_quote)
{
	if (token_value[i] == '\'' && !*s_quote)
		*s_quote = true;
	else if (token_value[i] == '\'' && *s_quote)
		*s_quote = false;
	if (token_value[i] == '\"' && !*d_quote)
		*d_quote = true;
	else if (token_value[i] == '\"' && *d_quote)
		*d_quote = false;
}

/**
 * @brief Expands variables within a token before further tokenization.
 *
 * Processes the given token_value by iterating through its characters, handling quotes,
 * and processing variable expansion where applicable. Literal characters are appended
 * directly, and variable references are expanded using process_token_character().
 *
 * @param token_value The original token string possibly containing variable references.
 * @param data Pointer to the minishell data structure.
 * @return char* The expanded token string, or NULL on failure.
 */
char	*expand_variables_pretoken(char *token_value, t_data *data)
{
	char	*expanded;
	int		i;
	bool	s_d_quote[2];

	i = 0;
	s_d_quote[0] = false;
	s_d_quote[1] = false;
	if (!token_value)
		return (NULL);
	expanded = ft_calloc(sizeof(char), (ft_strlen(token_value) + 1));
	if (!expanded)
		return (NULL);
	while (token_value[i] != '\0')
	{
		check_quotes(token_value, i, &s_d_quote[0], &s_d_quote[1]);
		if (s_d_quote[0] || s_d_quote[1])
		{
			if (!append_literal_char(token_value[i++], &expanded))
				return (NULL);
		}
		else if (process_token_character(token_value, &i, &expanded,
				data) == ERROR)
			return (NULL);
	}
	return (expanded);
}
