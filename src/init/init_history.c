/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_history.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 07:21:50 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/25 07:21:51 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
