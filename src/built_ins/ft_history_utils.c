#include "../../include/minishell.h"

/**
 * @brief Sets all elements of the history matrix to null.
 *
 * Iterates through the history matrix (with dimensions HISTORY_ROWS x HISTORY_COLUMNS)
 * and assigns the null character ('\0') to every element.
 *
 * @param matrix The history matrix to be cleared.
 */
void	set_matrix_null(char matrix[HISTORY_ROWS][HISTORY_COLUMNS])
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < HISTORY_ROWS)
	{
		j = 0;
		while (j < HISTORY_COLUMNS)
		{
			matrix[i][j] = '\0';
			j++;
		}
		i++;
	}
}

/**
 * @brief Deletes the history file and resets history data.
 *
 * Clears the readline history, sets the history matrix to null,
 * resets the history size to 0, closes the history file descriptor,
 * and writes an empty string to the history file.
 *
 * @param data Pointer to the minishell data structure.
 */
void	delete_history_file(t_data *data)
{
	rl_clear_history();
	set_matrix_null(data->history);
	data->hist_size = 0;
	close(data->fd);
	ft_write_history(data, "");
}

/**
 * @brief Prints the history.
 *
 * Retrieves the user's home directory path, opens the history file in read-only mode,
 * reads each line from the file, and prints it to standard output with a line number prefix.
 * Frees allocated memory and closes the file descriptor when done.
 *
 * @param data Pointer to the minishell data structure.
 */
void	print_history(t_data *data)
{
	int		i;
	char	*line;
	char	*path;

	i = 0;
	path = ft_get_user_home(data);
	data->fd = open(path, O_RDONLY);
	line = get_next_line(data->fd);
	while (line)
	{
		i++;
		printf("%d %s", i, line);
		free(line);
		line = get_next_line(data->fd);
	}
	free(path);
	close(data->fd);
}

/**
 * @brief Checks if a given line exists in the history.
 *
 * Compares the provided line with the most recent history entry to determine if it exists.
 *
 * @param line The line to search for in the history.
 * @param data Pointer to the minishell data structure.
 * @return true If the line exists in the last entry of the history.
 * @return false Otherwise.
 */
bool	exist_on_history(char *line, t_data *data)
{
	if (ft_strnstr(data->history[data->hist_size - 1], line,
			ft_strlen(data->history[data->hist_size - 1])) != NULL)
		return (true);
	return (false);
}
