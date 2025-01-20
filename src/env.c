/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 11:13:07 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/20 13:57:15 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	set_exp(t_data *data, char *name, char *value)
{
	int	i;

	i = 0;
	while (data->envp[i])
		i++;
	data->envp = ft_realloc(data->envp, sizeof(char *) * (i + 2));
	data->envp[i] = malloc(ft_strlen(name) + ft_strlen(value) + 2);
	if (data->envp[i] == NULL)
	{
		perror("malloc");
		return (ERROR); //OJO (SALIMOS?)
	}
	ft_strcpy(data->envp[i], name);
	ft_strcat(data->envp[i], "=");
	ft_strcat(data->envp[i], value);
	data->envp[i + 1] = NULL;
	return (OK);
}

void	set_last_cmd_arg(t_data *data, char *name, char *value)
{
	int	i;

	i = 0;
	while (data->envp[i] != NULL)
	{
		if (ft_strncmp(data->envp[i], name, ft_strlen(name)) == 0
			&& data->envp[i][ft_strlen(name)] == '=')
		{
			free(data->envp[i]);
			data->envp[i] = malloc(ft_strlen(name) + ft_strlen(value) + 2);
			if (data->envp[i] == NULL)
			{
				perror("malloc");
				return ;
			}
			ft_strcpy(data->envp[i], name);
			ft_strcat(data->envp[i], "=");
			ft_strcat(data->envp[i], value);
			return ;
		}
		i++;
	}
	if (set_exp(data, name, value) == ERROR)
		return ; //OJO (SALIMOS?)
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
