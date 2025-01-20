/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 11:20:00 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/20 12:59:20 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
		write(data->fd, "\n", 1);
		i++;
	}
	close(data->fd);
}

void	ft_write_history(t_data *data, char *line)
{
	int	i;

	i = 0;
	add_history(line);
	if (data->hist_size >= 1000)
	{
		while (i < 1000)
		{
			ft_strcpy(data->history[i], data->history[i + 1]);
			i++;
		}
		ft_strcpy(data->history[999], line);
		ft_save_history(data);
		return ;
	}
	ft_strcpy(data->history[data->hist_size], line);
	data->hist_size++;
	ft_save_history(data);
}

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

char	*ft_get_user_home(t_data *data)
{
	DIR				*dir;
	struct dirent	*entry;
	char			*h_path;
	char			*username;

	username = mini_getenv("HOME", data->envp);
	if (username != NULL)
		return (ft_strjoin(username, "/.minishell_history"));
	dir = opendir("/home");
	if (dir == NULL)
		return (NULL);
	while ((entry = readdir(dir)) != NULL)
	{
		if (ft_strncmp(entry->d_name, "Desktop", 8) != 0)
		{
			h_path = make_path(entry->d_name);
			if (h_path == NULL)
				return (closedir(dir), NULL);
			if (access(h_path, F_OK) == 0)
				return (closedir(dir), h_path);
			free(h_path);
		}
	}
	return (closedir(dir), NULL);
}
