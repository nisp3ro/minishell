#include "../../include/minishell.h"

/**
 * @brief Prompts the user to apologize for triggering the secret fork bomb.
 *
 * This function writes a newline and continuously prompts the user with a message
 * asking them to type "SORRY". When the user inputs "SORRY", it prints a reassuring
 * message and exits the loop.
 *
 * @param line A pointer to a line buffer (used to store user input).
 */
static void	say_sorry(char *line)
{
	write(1, "\n", 2);
	while (1)
	{
		line = readline("         Apologize for trying to break"
						"our minishell. Say \"SORRY\" -> ");
		if (line && ft_strncmp(line, "SORRY", 6) == 0)
		{
			write(1, "\nI'ts okay, you're welcome :)\n\n", 32);
			if (line)
				free(line);
			break ;
		}
		if (line)
			free(line);
	}
}

/**
 * @brief Initializes secret command arguments and opens the secret file.
 *
 * Sets up the command array with predefined arguments for the secret command and opens
 * the secret file located in "./.secret/secret.txt". If opening the file fails, it
 * prints an error message and exits with an error code.
 *
 * @param command An array of strings that will hold the secret command arguments.
 * @param fd Pointer to an integer where the file descriptor will be stored.
 */
static void	init_secret_vars(char *command[], int *fd)
{
	command[0] = "resize";
	command[1] = "-s";
	command[2] = "100";
	command[3] = "400";
	command[4] = NULL;
	*fd = open("./.secret/secret.txt", O_RDONLY, 0666);
	if (!*fd)
		return (perror("open"), exit(ERROR));
}

/**
 * @brief Executes the secret child process.
 *
 * In the child process, this function initializes secret command variables,
 * retrieves the PATH environment variable, finds the command in the PATH,
 * reads the secret file line by line, writes its contents to the standard output,
 * and then exits with OK status.
 *
 * @param data Pointer to the minishell data structure.
 * @param secret_line A pointer to a buffer that will hold lines read from the secret file.
 * @param envp The environment variable array.
 */
static void	child_secret(t_data *data, char *secret_line, char *envp[])
{
	int		fd;
	char	*command[5];
	char	*path;
	char	*resize_path;

	init_secret_vars(command, &fd);
	path = mini_getenv("PATH", envp);
	resize_path = find_command_in_path(data, command[0], envp);
	secret_line = get_next_line(fd);
	while (secret_line)
	{
		write(1, secret_line, ft_strlen(secret_line));
		free(secret_line);
		secret_line = get_next_line(fd);
	}
	close(fd);
	exit(OK);
}

/**
 * @brief Detects a fork bomb attempt and handles it as a secret easter egg.
 *
 * This function checks if the provided line (after trimming leading spaces) matches
 * the fork bomb pattern (":(){ :|:& };:" or "forkbomb"). If a match is found, it forks
 * a child process to execute the secret behavior (printing secret file content), waits for
 * the child process to finish, frees the input line, and then prompts the user to apologize.
 *
 * @param data Pointer to the minishell data structure.
 * @param envp The environment variable array.
 * @param line The input command line.
 * @return true If a fork bomb was detected and handled.
 * @return false Otherwise.
 */
bool	fork_bomb(t_data *data, char *envp[], char *line)
{
	int		i;
	pid_t	pid;
	char	*secret_line;

	i = 0;
	secret_line = NULL;
	while (isspace((unsigned char)line[i]))
		i++;
	if (ft_strncmp((line + i), ":(){ :|:& };:", 13) == 0 || ft_strncmp((line + i),
			"forkbomb", 8) == 0)
	{
		pid = fork();
		if (pid == 0)
			child_secret(data, secret_line, envp);
		waitpid(pid, NULL, 0);
		free(line);
		say_sorry(line);
		data->g_exit_code = 0;
		return (true);
	}
	return (false);
}
