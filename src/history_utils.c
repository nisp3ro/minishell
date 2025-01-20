/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvidal-t <jvidal-t@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 15:12:06 by jvidal-t          #+#    #+#             */
/*   Updated: 2025/01/20 19:24:55 by jvidal-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

void	delete_history_file(t_data *data)
{
	rl_clear_history();
	set_matrix_null(data->history);
	data->hist_size = 0;
	close(data->fd);
	ft_write_history(data, "");
}

void	print_history(t_data *data)
{
	char	*line;
	char	*path;

	path = ft_get_user_home(data);
	data->fd = open(path, O_RDONLY);
	line = get_next_line(data->fd);
	while (line)
	{
		write(1, line, ft_strlen(line));
		free(line);
		line = get_next_line(data->fd);
	}
	free(path);
	close(data->fd);
}

bool	exist_on_history(char *line, t_data *data)
{
	if (ft_strnstr(data->history[data->hist_size - 1], line,
			ft_strlen(data->history[data->hist_size - 1])) != NULL)
		return (true);
	return (false);
}
