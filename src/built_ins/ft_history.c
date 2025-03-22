#include "../../include/minishell.h"

/**
 * @brief Saves the history to the history file.
 *
 * This function retrieves the user's history file path using ft_get_user_home,
 * opens the file in read/write mode (creating/truncating it if necessary), and
 * writes the in-memory history stored in data->history to the file. Each history
 * line is followed by a newline if the line is not empty.
 *
 * @param data Pointer to the minishell data structure.
 */
void	ft_save_history(t_data *data)
{
	int		i;
	char	*usr_path;

	usr_path = ft_get_user_home(data);
	if (usr_path == NULL)
		return ;
	data->fd = open(usr_path, O_RDWR | O_CREAT | O_TRUNC, 0644);
	free(usr_path);
	if (data->fd == -1)
		return ;
	i = 0;
	while (i < data->hist_size)
	{
		write(data->fd, data->history[i], ft_strlen(data->history[i]));
		if (data->history[i][0] != '\0')
			write(data->fd, "\n", 1);
		i++;
	}
	close(data->fd);
}

/**
 * @brief Writes a line to the history.
 *
 * Checks if the provided line already exists in the history using exist_on_history.
 * If it does not exist or the line is empty, it adds the line to the history using
 * add_history, updates the in-memory history matrix stored in data->history, and then
 * saves the history to the file using ft_save_history.
 *
 * @param data Pointer to the minishell data structure.
 * @param line The history line to write.
 */
void	ft_write_history(t_data *data, char *line)
{
	int	i;

	i = 0;
	if (exist_on_history(line, data) == false || line[0] == '\0' )
	{
		add_history(line);
		if (data->hist_size >= HISTORY_ROWS)
		{
			while (i < HISTORY_ROWS - 1)
			{
				ft_strcpy(data->history[i], data->history[i + 1]);
				i++;
			}
			ft_strcpy(data->history[HISTORY_ROWS - 1], line);
		}
		else
		{
			ft_strcpy(data->history[data->hist_size], line);
			data->hist_size++;
		}
		ft_save_history(data);
	}
}

/**
 * @brief Constructs a history file path based on the user's name.
 *
 * Creates a new path string in the format:
 * "/home/{name}/.minishell_history"
 *
 * @param name The user's name.
 * @return char* A pointer to the newly allocated path string, or NULL on failure.
 */
char	*make_path(char *name)
{
	char	*new_path;
	int		len;

	len = ft_strlen("/home/") + ft_strlen(name)
		+ ft_strlen("/.minishell_history") + 1;
	new_path = malloc(len);
	if (new_path == NULL)
		return (NULL);
	ft_strcpy(new_path, "/home/");
	ft_strcat(new_path, name);
	ft_strcat(new_path, "/.minishell_history");
	return (new_path);
}

/**
 * @brief Retrieves the user's history file path.
 *
 * First, the function attempts to get the home directory from the environment variable
 * "HOME" via mini_getenv. If that is not available, it opens the "/home" directory and
 * searches for a user directory (skipping entries like "Desktop"). If a valid user directory
 * is found and the corresponding history file exists, it returns the path to that file.
 *
 * @param data Pointer to the minishell data structure.
 * @return char* A pointer to the history file path, or NULL if not found.
 */
char	*ft_get_user_home(t_data *data)
{
	DIR				*dir;
	struct dirent	*entry;
	char			*user_home[2];

	user_home[0] = mini_getenv("HOME", data->envp);
	if (user_home[0] != NULL)
		return (ft_strjoin(user_home[0], "/.minishell_history"));
	dir = opendir("/home");
	if (dir == NULL)
		return (NULL);
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (ft_strncmp(entry->d_name, "Desktop", 8) != 0)
		{
			user_home[1] = make_path(entry->d_name);
			if (user_home[1] == NULL)
				return (closedir(dir), NULL);
			if (access(user_home[1], F_OK) == 0)
				return (closedir(dir), user_home[1]);
			free(user_home[1]);
		}
		entry = readdir(dir);
	}
	return (closedir(dir), NULL);
}
