#include "../../include/minishell.h"

/**
 * @brief Processes a single command input.
 *
 * This function trims leading spaces, checks if the command starts correctly,
 * handles unfinished pipes, cleans the input line, expands variables before tokenizing,
 * and finally parses and executes the command if it's not empty.
 *
 * @param input The raw input string entered by the user.
 * @param data Pointer to the minishell data structure.
 * @return int Returns OK if the command was processed successfully, or ERROR otherwise.
 */
int	process_command(char *input, t_data *data)
{
	char	*line[2];
	int		i;

	i = 0;
	while (isspace((unsigned char)input[i]))
		i++;
	if (check_cmd_start(data, input, i) == ERROR)
		return (ERROR);
	line[0] = unfinished_pipe(input);
	if (line[0] == NULL)
		return (ERROR);
	line[1] = clean_line(line[0], data);
	if (line[1] == NULL)
		return (ERROR);
	line[0] = expand_variables_pretoken(line[1], data);
	free(line[1]);
	if (line[0] == NULL)
		return (ERROR);
	if (line[0][0] != '\0')
		token_parsec_exec(line[0], data, true);
	else
		free(line[0]);
	return (OK);
}

/**
 * @brief Runs the interactive mode of the minishell.
 *
 * This function continuously prompts the user for input, processes the command,
 * and handles the fork bomb easter egg. It loops indefinitely until an EOF
 * condition is encountered (e.g., Ctrl-D), at which point it prints "exit" and returns.
 *
 * @param data Pointer to the minishell data structure.
 * @param envp The environment variable array.
 * @return int The global exit code after shell termination.
 */
int	interactive_mode(t_data *data, char *envp[])
{
	char	*line;

	while (1)
	{
		if (get_prompt(&data->prompt, data) == ERROR)
			return (ERROR);
		line = readline(data->prompt);
		free(data->prompt);
		data->prompt = NULL;
		if (!line)
			return (printf("exit\n"), data->g_exit_code);
		if (fork_bomb(data, envp, line))
			continue ;
		if (process_command(line, data) == ERROR)
			continue ;
	}
	return (data->g_exit_code);
}
