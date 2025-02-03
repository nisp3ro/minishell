/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_envp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 07:21:54 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/02/03 09:05:10 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*get_path_line(void)
{
	int		fd;
	char	*line;
	char	*tmp;

	fd = open("/etc/environment", O_RDONLY);
	if (fd < 0)
		return (NULL);
	line = get_next_line(fd);
	tmp = get_next_line(fd);
	while (tmp)
	{
		free(tmp);
		tmp = get_next_line(fd);
	}
	close(fd);
	return (line);
}

int	create_path_no_env(t_data *data)
{
	char	*tmp;
	char	*line;

	if (access("/etc/environment", F_OK) == 0)
	{
		line = get_path_line();
		if (line)
		{
			tmp = NULL;
			tmp = ft_strchr(line, '\n');
			if (tmp)
				*tmp = '\0';
			tmp = NULL;
			tmp = ft_strchr(line, '=');
			if (tmp)
				*tmp = '\0';
			tmp = ft_strtrim(tmp + 1, "\"");
			if (tmp == NULL)
				return (free(line), ERROR);
			if (set_variable(&data->vars, line, tmp) == ERROR)
				return (free(line), free(tmp), ERROR);
			return (free(line), free(tmp), OK);
		}
	}
	return (OK);
}

int	update_envp(t_data *data)
{
	int		i;
	int		lvl;
	char	*tmp;

	i = -1;
	tmp = NULL;
	while (data->envp[++i])
	{
		if (ft_strncmp(data->envp[i], "SHLVL=", 6) == 0)
		{
			lvl = ft_atoi(data->envp[i] + 6);
			free(data->envp[i]);
			tmp = ft_itoa(++lvl);
			if (!tmp)
				return (ERROR);
			data->envp[i] = ft_strjoin("SHLVL=", tmp);
			if (!data->envp[i])
				return (free(tmp), ERROR);
			return (free(tmp), OK);
		}
	}
	if (!data->envp[i])
		if (set_exp(data, "SHLVL", "1") == ERROR)
			return (ERROR);
	return (OK);
}

char	**cpy_env(char *envp[])
{
	char	**cpy;
	int		i;

	i = 0;
	while (envp[i])
		i++;
	cpy = malloc(sizeof(char *) * (i + 1));
	if (cpy == NULL)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		cpy[i] = ft_strdup(envp[i]);
		if (!cpy[i])
		{
			while (i >= 0)
				free(cpy[i--]);
			free(cpy);
			return (NULL);
		}
		i++;
	}
	cpy[i] = NULL;
	return (cpy);
}

int	create_envp(t_data *data)
{
	char	*tmp_pwd;
	char	*tmp_cwd;

	data->envp = malloc(sizeof(char *) * 4);
	if (data->envp == NULL)
		return (clean_mtx(data->envp), ERROR);
	tmp_pwd = ft_strdup("PWD=");
	if (tmp_pwd == NULL)
		return (clean_mtx(data->envp), ERROR);
	tmp_cwd = getcwd(NULL, 0);
	if (tmp_cwd == NULL)
		return (free(tmp_pwd), clean_mtx(data->envp), ERROR);
	data->envp[0] = ft_strjoin(tmp_pwd, tmp_cwd);
	free(tmp_pwd);
	free(tmp_cwd);
	if (data->envp[0] == NULL)
		return (clean_mtx(data->envp), ERROR);
	data->envp[1] = ft_strdup("SHLVL= 1");
	if (data->envp[1] == NULL)
		return (clean_mtx(data->envp), ERROR);
	data->envp[2] = ft_strdup("_=]");
	if (data->envp[2] == NULL)
		return (clean_mtx(data->envp), ERROR);
	data->envp[3] = NULL;
	return (OK);
}
