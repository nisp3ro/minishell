#include "../../include/minishell.h"

/**
 * @brief Initializes the history matrix and opens the history file.
 *
 * This function initializes the history matrix (a 1001x1024 char array) by setting all 
 * its elements to '\0'. It then obtains the user's history file path using ft_get_user_home, 
 * opens the file in read-only mode, and assigns the file descriptor to data->fd. The history 
 * size (data->hist_size) is set to 0.
 *
 * @param history The 2D char array to hold the history lines.
 * @param data Pointer to the minishell data structure.
 * @return int Returns OK on success or ERROR if any step fails.
 */
int	ft_init_history(char history[1001][1024], t_data *data)
{
	int		i;
	int		j;
	char	*usr_path;

	data->hist_size = 0;
	usr_path = ft_get_user_home(data);
	if (usr_path == NULL)
		return (ERROR);
	data->fd = open(usr_path, O_RDONLY);
	free(usr_path);
	if (data->fd == -1)
		return (ERROR);
	i = 0;
	while (i < 1001)
	{
		j = 0;
		while (j < 1024)
		{
			history[i][j] = '\0';
			j++;
		}
		i++;
	}
	return (OK);
}

/**
 * @brief Recovers the history from the history file into memory.
 *
 * This function initializes the history matrix by calling ft_init_history. If successful,
 * it reads each line from the history file using get_next_line, copies the line into the
 * history matrix (data->history), removes any newline character from the end of the line,
 * and adds the line to the history list using add_history. The history size counter (data->hist_size)
 * is incremented for each line read. Finally, the history file descriptor is closed.
 *
 * @param data Pointer to the minishell data structure.
 */
void	ft_recover_history(t_data *data)
{
	int		i;
	char	*line;

	if (ft_init_history(data->history, data) == OK)
	{
		line = get_next_line(data->fd);
		while (line != NULL)
		{
			ft_strcpy(data->history[data->hist_size], line);
			i = -1;
			while (data->history[data->hist_size][++i] != '\0')
			{
				if (data->history[data->hist_size][i] == '\n')
				{
					data->history[data->hist_size][i] = '\0';
					break ;
				}
			}
			add_history(data->history[data->hist_size]);
			free(line);
			line = get_next_line(data->fd);
			data->hist_size++;
		}
	}
	close(data->fd);
}
