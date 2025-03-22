#include "../../include/minishell.h"

/**
 * @brief Sets the last command argument for export.
 *
 * If the command is "export" with no arguments, it sets the last argument
 * to the value of the command itself. Otherwise, it sets the last argument
 * to the last element in the command's arguments array.
 *
 * @param commands Pointer to the command structure.
 * @param data Pointer to the minishell data structure.
 */
void	parse_last_cmd_arg(t_command *commands, t_data *data)
{
	int	i;

	if (ft_strncmp(commands->args[0], "export", 7) == 0
		&& commands->args[1] == NULL)
		set_last_cmd_arg(data, "_", commands->args[0]);
	else if (ft_strncmp(commands->args[0], "export", 7) != 0)
	{
		i = 0;
		while (commands->args[i])
			i++;
		set_last_cmd_arg(data, "_", commands->args[i - 1]);
	}
}

/**
 * @brief Checks if the command line starts with an unexpected token.
 *
 * If the first non-space character of the line is a pipe ('|'),
 * an error message is printed, the global exit code is set to 2, the line is freed,
 * and ERROR is returned.
 *
 * @param data Pointer to the minishell data structure.
 * @param line The command line input.
 * @param i The starting index (after skipping leading spaces).
 * @return int Returns OK if the line starts correctly, or ERROR if there is a syntax error.
 */
int	check_cmd_start(t_data *data, char *line, int i)
{
	if (line[i] == '|')
	{
		write(STDERR_FILENO,
			"minishell: syntax error near unexpected token `|'\n",
			51);
		data->g_exit_code = 2;
		free(line);
		return (ERROR);
	}
	return (OK);
}

/**
 * @brief Tokenizes, parses, and executes a command pipeline.
 *
 * This function tokenizes the full command string, parses it into a command pipeline,
 * and then either executes a built-in command (if present) or runs the pipeline.
 * In interactive mode, the full command string is freed after tokenization.
 *
 * @param full_cmd The full command string to be processed.
 * @param data Pointer to the minishell data structure.
 * @param interactive Boolean flag indicating if the shell is in interactive mode.
 */
void	token_parsec_exec(char *full_cmd, t_data *data, bool interactive)
{
	t_token		*tokens;
	t_command	*commands;

	tokens = tokenize(full_cmd, data);
	if (interactive)
		free(full_cmd);
	if (tokens == NULL)
		return ;
	commands = parse_pipeline(data, tokens);
	free_tokens(tokens);
	if (commands == NULL)
		return ;
	if (commands->args && !commands->next)
		parse_last_cmd_arg(commands, data);
	if ((!commands || !commands->next) && check_builtin_prepipe(commands,
			data) == true)
		return (clean_cmd(commands));
	else
		execute_pipeline(commands, data, data->envp);
}

/**
 * @brief Prompts the user for additional input to complete an unfinished pipe.
 *
 * Displays a secondary prompt (">") and reads the additional input.
 * If the user does not provide any input (i.e., EOF is reached), it prints a syntax error,
 * frees the current line, and exits with status 2.
 *
 * @return char* The additional input line, or NULL if an error occurs.
 */
static char	*fill_pipe(char *line)
{
	char	*tmp;

	tmp = readline(">");
	if (!tmp)
	{
		write(STDERR_FILENO,
			"minishell: syntax error near unexpected token `|'\n",
			51);
		free(line);
		exit (2);
	}
	return (tmp);
}

/**
 * @brief Handles unfinished pipe commands by appending additional input.
 *
 * If the input line ends with a pipe ('|') (after trimming trailing spaces),
 * this function repeatedly prompts the user for additional input to complete
 * the command. The additional input is appended to the existing line until
 * the pipe is no longer the last non-space character.
 *
 * @param line The initial input line.
 * @return char* The completed input line with all appended pipe continuations.
 */
char	*unfinished_pipe(char *line)
{
	char	*tmp[2];
	int		i;

	while (1)
	{
		i = ft_strlen(line) - 1;
		while (i > 0 && isspace((unsigned char)line[i]))
			i--;
		if (i >= 0 && line[i] == '|')
		{
			tmp[0] = fill_pipe(line);
			tmp[1] = ft_strjoin(line, " ");
			if (tmp[1] == NULL)
				return (free(tmp[0]), free(line), NULL);
			free(line);
			line = ft_strjoin(tmp[1], tmp[0]);
			if (line == NULL)
				return (free(tmp[0]), free(tmp[1]), NULL);
			free(tmp[0]);
			free(tmp[1]);
		}
		else
			break ;
	}
	return (line);
}
