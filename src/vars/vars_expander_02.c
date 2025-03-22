#include "../../include/minishell.h"

/**
 * @brief Expands a variable reference in the token value.
 *
 * Starting at index *i (which points to a '$'), this function extracts the variable name
 * by scanning alphanumeric characters and underscores. It then retrieves the variable's value
 * from the environment or internal variables. The retrieved value is appended to the string
 * pointed to by *expanded. The variable name (including the '$') is temporarily stored in a
 * buffer, which is freed before returning.
 *
 * @param token_value The token string containing the variable reference.
 * @param i Pointer to the current index in token_value; updated to the end of the variable name.
 * @param expanded Pointer to the accumulated expanded string.
 * @param data Pointer to the minishell data structure.
 * @return char* The updated expanded string, or NULL on failure.
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
	while ((token_value[j] != '\0' && ft_isalnum(token_value[j]))
		|| (token_value[j] != '\0' && token_value[j] == '_'))
		j++;
	var_val[0] = malloc(sizeof(char) * (j - *i));
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
 * @brief Expands the special variable '$?' with the exit code.
 *
 * Converts the global exit code to a string using ft_itoa, appends it to the string
 * pointed to by *expanded, and advances the index *i by 2 (skipping "$?").
 *
 * @param data Pointer to the minishell data structure.
 * @param i Pointer to the current index in the token string; will be advanced by 2.
 * @param expanded Pointer to the accumulated expanded string.
 * @return char* The updated expanded string, or NULL on failure.
 */
char	*handle_exit_code_expansion(t_data *data, int *i, char **expanded)
{
	char	*tmp;
	char	*tmp2;

	tmp = ft_itoa(data->g_exit_code);
	if (!tmp)
	{
		perror("Error");
		exit(1);
	}
	tmp2 = ft_strjoin(*expanded, tmp);
	free(tmp);
	free(*expanded);
	*expanded = tmp2;
	*i += 2;
	return (*expanded);
}

/**
 * @brief Appends a literal character to the expanded string.
 *
 * Allocates a temporary string containing the single character, appends it to *expanded
 * using ft_strjoin, frees the temporary string and the old expanded string, and updates
 * *expanded with the new string.
 *
 * @param c The literal character to append.
 * @param expanded Pointer to the accumulated expanded string.
 * @return char* The updated expanded string, or NULL on failure.
 */
char	*append_literal_char(char c, char **expanded)
{
	char	*tmp;
	char	*tmp2;

	tmp = malloc(sizeof(char) * 2);
	if (!tmp)
	{
		free(*expanded);
		return (NULL);
	}
	tmp[0] = c;
	tmp[1] = '\0';
	tmp2 = ft_strjoin(*expanded, tmp);
	free(tmp);
	free(*expanded);
	*expanded = tmp2;
	return (*expanded);
}

/**
 * @brief Processes a single character or variable reference in the token.
 *
 * Depending on the characters at the current index in token_value:
 * - If a '$' is followed by an alphanumeric character or '_', calls handle_variable_expansion().
 * - If a '$' is followed by '?', calls handle_exit_code_expansion().
 * - Otherwise, appends the literal character using append_literal_char().
 * Advances the index accordingly.
 *
 * @param token_value The token string being processed.
 * @param i Pointer to the current index in token_value.
 * @param expanded Pointer to the accumulated expanded string.
 * @param data Pointer to the minishell data structure.
 * @return int Returns OK on success, or ERROR on failure.
 */
static int	process_token_character(char *token_value, int *i, char **expanded,
		t_data *data)
{
	if ((token_value[*i] == '$' && ft_isalnum(token_value[*i + 1]))
		|| (token_value[*i] == '$' && token_value[*i + 1] == '_'))
	{
		if (!handle_variable_expansion(token_value, i, expanded, data))
			return (ERROR);
	}
	else if (token_value[*i] == '$' && token_value[*i + 1] == '?')
	{
		if (!handle_exit_code_expansion(data, i, expanded))
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
 * @brief Expands variable references in a token string.
 *
 * Allocates a new string to accumulate the expanded result and iterates through the
 * input token_value character by character. For each character, it processes variable
 * expansions or appends literal characters using process_token_character(). Returns the
 * fully expanded string.
 *
 * @param token_value The original token string containing variable references.
 * @param data Pointer to the minishell data structure.
 * @return char* The expanded token string, or NULL on failure.
 */
char	*expand_variables(char *token_value, t_data *data)
{
	char	*expanded;
	int		i;

	i = 0;
	if (!token_value)
		return (NULL);
	expanded = ft_calloc(sizeof(char), (ft_strlen(token_value) + 1));
	if (!expanded)
		return (NULL);
	while (token_value[i] != '\0')
	{
		if (process_token_character(token_value, &i, &expanded, data) == ERROR)
			return (NULL);
	}
	return (expanded);
}
