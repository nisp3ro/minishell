#include "../../include/minishell.h"

/**
 * @brief Processes the lines for the here-document.
 *
 * This function continuously reads lines for a here-document until the limiter is reached.
 * It checks if the current line matches the limiter (comparing with line[1]), and if so,
 * frees the line and terminates the loop. If the line contains variable references ('$')
 * and the limiter is not quoted, the variables are expanded before writing the line
 * to the here-document pipe.
 *
 * @param data Pointer to the minishell data structure.
 * @param line Pointer to an array of two strings: line[0] is the current input line,
 *             and line[1] holds the processed limiter.
 * @param limiter The limiter string that signals the end of the here-document.
 * @param fd Pointer to an integer array representing the file descriptors of the pipe.
 *           fd[1] is used for writing the here-document contents.
 */
void	manage_here_doc(t_data *data, char **line, char *limiter, int *fd)
{
	while (line[0])
	{
		if ((ft_strncmp(line[0], line[1], (ft_strlen(line[1]) + 1)) == 0)
			|| !line[0])
		{
			if (line[0])
				free(line[0]);
			break ;
		}
		if (ft_strchr(line[0], '$') && ((limiter[1] != '\''
					&& limiter[ft_strlen(limiter) - 1] != '\'')
				&& (limiter[1] != '\"' && limiter[ft_strlen(limiter)
						- 1] != '\"')))
			line[0] = expand_variables(line[0], data);
		write(fd[1], line[0], ft_strlen(line[0]));
		write(fd[1], "\n", 1);
		free(line[0]);
		line[0] = readline("> ");
	}
}

/**
 * @brief Reads input for the here-document and writes it to a pipe.
 *
 * This static function prompts the user with a ">" prompt and reads the input line.
 * It processes the limiter: if the limiter is quoted, the quotes are trimmed; otherwise,
 * it duplicates the limiter. The function then calls manage_here_doc() to handle the
 * here-document input, and finally frees the allocated limiter copy.
 *
 * @param data Pointer to the minishell data structure.
 * @param limiter The limiter string that marks the end of the here-document.
 * @param fd Pointer to an integer array representing the file descriptors of the pipe.
 */
static void	read_n_write(t_data *data, char *limiter, int *fd)
{
	char	*line[2];

	line[0] = NULL;
	line[0] = readline("> ");
	if ((limiter[1] != '\'' && limiter[ft_strlen(limiter) - 1] != '\'')
		|| (limiter[1] != '\"' && limiter[ft_strlen(limiter) - 1] != '\"'))
	{
		if (limiter[0] == '\'')
			line[1] = ft_strtrim(limiter, "\'");
		else
			line[1] = ft_strtrim(limiter, "\"");
	}
	else
		line[1] = ft_strdup(limiter);
	manage_here_doc(data, line, limiter, fd);
	free(line[1]);
}

/**
 * @brief Handles the here-document functionality.
 *
 * This function forks a child process to read here-document input. The child process
 * waits for a signal, closes the read end of the pipe, and calls read_n_write() to read
 * and write the here-document content. After writing, the child closes the write end and
 * exits. The parent process waits for the child to finish.
 *
 * @param data Pointer to the minishell data structure.
 * @param limiter The limiter string that marks the end of the here-document.
 * @param fd Pointer to an integer array representing the file descriptors of the pipe.
 */
void	here_doc(t_data *data, char *limiter, int *fd)
{
	pid_t	reader;

	data->g_exit_code = 0;
	data->g_exit_code = getpid();
	reader = fork();
	if (reader == -1)
		exit(1);
	if (reader == 0)
	{
		wait_signal(2);
		close(fd[0]);
		read_n_write(data, limiter, fd);
		close(fd[1]);
		exit(EXIT_SUCCESS);
	}
	waitpid(reader, &data->g_exit_code, 0);
}

/**
 * @brief Handles all here-document inputs for a command.
 *
 * For each here-document delimiter present in the command's EOF list, this function
 * sets up a pipe and calls here_doc() to process the here-document input. If the global
 * exit code is set to SIGKILL, it is adjusted to 130. Finally, it closes the write end
 * of the here-document pipe.
 *
 * @param command Pointer to the command structure containing here-document delimiters.
 * @param data Pointer to the minishell data structure.
 * @param here_doc_pipe Pointer to an integer array representing the file descriptors of the pipe.
 */
void	handle_here_doc(t_command *command, t_data *data, int *here_doc_pipe)
{
	int	eof_i;

	if (pipe(here_doc_pipe) == -1)
		exit(1);
	eof_i = -1;
	while (command->eof[++eof_i])
		here_doc(data, command->eof[eof_i], here_doc_pipe);
	if (data->g_exit_code == SIGKILL)
		data->g_exit_code = 130;
	close(here_doc_pipe[1]);
}
