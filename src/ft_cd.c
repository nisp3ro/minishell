/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 13:25:06 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/20 13:25:14 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_cd(t_command *command, t_data *data)
{
	int		i;
	int		pwd_found;
	int		oldpwd_found;
	char	*tmp;
	char	*tmp2;

	i = 0;
	pwd_found = 0;
	oldpwd_found = 0;
	if (!command->args[1])
	{
		tmp = mini_getenv("HOME", data->envp);
		if (chdir(tmp) == -1)
		{
			perror("cd");
			return ;
		}
	}
	else if (command->args[2])
	{
		write(STDERR_FILENO, " too many arguments\n", 20);
		g_error = 1;
		return ;
	}
	else if (command->args[1][0] == '~')
	{
		tmp2 = mini_getenv("HOME", data->envp);
		tmp = ft_strjoin(tmp2, command->args[1] + 1);
		if (chdir(tmp) == -1)
		{
			perror("cd");
			g_error = 1;
			return ;
		}
	}
	else if (ft_strncmp(command->args[1], "-", 2) == 0)
	{
		if (chdir(data->oldpwd) == -1)
		{
			perror("cd");
			g_error = 1;
			return ;
		}
	}
	else if (chdir(command->args[1]) == -1)
	{
		perror("cd");
		g_error = 1;
		return ;
	}
	if (!data->oldpwd)
		free(data->oldpwd);
	data->oldpwd = data->pwd;
	data->pwd = getcwd(NULL, 0);
	if (data->pwd == NULL)
	{
		perror("getcwd");
		g_error = 1;
		return ;
	}
	while (data->envp[i] != NULL)
	{
		if (ft_strncmp(data->envp[i], "PWD=", 4) == 0)
		{
			free(data->envp[i]);
			data->envp[i] = malloc(ft_strlen("PWD=") + ft_strlen(data->pwd)
					+ 1);
			if (data->envp[i] == NULL)
			{
				perror("malloc");
				return ;
			}
			ft_strcpy(data->envp[i], "PWD=");
			ft_strcat(data->envp[i], data->pwd);
			pwd_found = 1;
			break ;
		}
		i++;
	}
	if (!pwd_found)
	{
		data->envp[i] = malloc(ft_strlen("PWD=") + ft_strlen(data->pwd) + 1);
		if (data->envp[i] == NULL)
		{
			perror("malloc");
			return ;
		}
		ft_strcpy(data->envp[i], "PWD=");
		ft_strcat(data->envp[i], data->pwd);
		data->envp[i + 1] = NULL;
	}
	i = 0;
	while (data->envp[i] != NULL)
	{
		if (ft_strncmp(data->envp[i], "OLDPWD=", 7) == 0)
		{
			free(data->envp[i]);
			data->envp[i] = malloc(ft_strlen("OLDPWD=")
					+ ft_strlen(data->oldpwd) + 1);
			if (data->envp[i] == NULL)
			{
				perror("malloc");
				return ;
			}
			ft_strcpy(data->envp[i], "OLDPWD=");
			ft_strcat(data->envp[i], data->oldpwd);
			oldpwd_found = 1;
			break ;
		}
		i++;
	}
	if (!oldpwd_found)
	{
		data->envp[i] = malloc(ft_strlen("OLDPWD=") + ft_strlen(data->oldpwd)
				+ 1);
		if (data->envp[i] == NULL)
		{
			perror("malloc");
			return ;
		}
		ft_strcpy(data->envp[i], "OLDPWD=");
		ft_strcat(data->envp[i], data->oldpwd);
		data->envp[i + 1] = NULL;
	}
	g_error = 0;
}
