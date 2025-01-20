/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvidal-t <jvidal-t@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 16:42:19 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/20 15:48:02 by jvidal-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

int	create_exp_vars(t_data *data, t_vars **exp_v, int flag)
{
	int		i;
	char	*var;
	char	*equal_sign;

	i = 0;
	while (data->envp[i + flag])
	{
		var = ft_strdup(data->envp[i]);
		if (var == NULL)
			return (ERROR);
		equal_sign = ft_strchr(var, '=');
		*equal_sign = '\0';
		equal_sign++;
		if (set_variable(exp_v, var, equal_sign) == ERROR)
		{
			free(var);
			clean_variables(*exp_v);
			return (ERROR);
		}
		free(var);
		i++;
	}
	return (OK);
}

int	handle_no_env(t_data *data)
{
	if (create_envp(data) == ERROR)
		return (ERROR);
	if (create_exp_vars(data, &data->exp_vars, 0))
		return (clean_mtx(data->envp), ERROR);
	return (OK);
}

int	init_data(t_data *data, char *envp[])
{
	data->exp_vars = NULL;
	if (envp == NULL || envp[0] == NULL || envp[0][0] == '\0')
		handle_no_env(data);
	else
	{
		data->envp = cpy_env(envp);
		if (data->envp == NULL)
			return (ERROR);
		if (update_envp(data) == ERROR)
			return (clean_mtx(data->envp), ERROR);
		if (create_exp_vars(data, &data->exp_vars, 1))
			return (clean_mtx(data->envp), ERROR);
	}
	sort_list(&data->exp_vars, data->exp_vars);
	data->home = NULL;
	data->pwd = getcwd(NULL, 0);
	if (!data->pwd)
		return (clean_mtx(data->envp), clean_variables(data->exp_vars), ERROR);
	data->oldpwd = getcwd(NULL, 0);
	if (!data->oldpwd)
		return (clean_mtx(data->envp), clean_variables(data->exp_vars),
			free(data->pwd), ERROR);
	data->prompt = NULL;
	data->vars = NULL;
	return (ft_recover_history(data), OK);
}
