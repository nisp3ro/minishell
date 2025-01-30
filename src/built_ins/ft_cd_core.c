/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_core.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 13:25:06 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/30 13:12:28 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static bool	handle_cd_path(char *path, t_data *data)
{
	if (chdir(path) == -1)
	{
		perror("cd");
		data->g_exit_code = 1;
		return (false);
	}
	return (true);
}

static bool	handle_special_paths(t_command *command, t_data *data)
{
	char	*home;
	char	*tmp;

	if (!command->args[1])
	{
		home = mini_getenv("HOME", data->envp);
		if (!home)
			return (perror("cd: HOME not set"), false);
		return (handle_cd_path(home, data));
	}
	else if (command->args[1][0] == '~')
	{
		home = mini_getenv("HOME", data->envp);
		if (!home)
			return (perror("cd: HOME not set"), false);
		tmp = ft_strjoin(home, command->args[1] + 1);
		if (!tmp || !handle_cd_path(tmp, data))
			return (free(tmp), false);
		free(tmp);
		return (true);
	}
	else if (ft_strncmp(command->args[1], "-", 2) == 0)
		return (handle_cd_path(data->oldpwd, data));
	return (handle_cd_path(command->args[1], data));
}

static void	update_pwd_and_oldpwd(t_data *data)
{
	char	*current_pwd;

	current_pwd = getcwd(NULL, 0);
	if (!current_pwd)
	{
		perror("getcwd");
		data->g_exit_code = 1;
		return ;
	}
	if (data->oldpwd)
		free(data->oldpwd);
	data->oldpwd = data->pwd;
	update_env_var(data, "OLDPWD", data->oldpwd);
	data->pwd = current_pwd;
	update_env_var(data, "PWD", data->pwd);
}

void	ft_cd(t_command *command, t_data *data)
{
	if (command->args[2])
	{
		write(STDERR_FILENO, "cd: too many arguments\n", 23);
		data->g_exit_code = 1;
		return ;
	}
	if (!handle_special_paths(command, data))
		return ;
	update_pwd_and_oldpwd(data);
	data->g_exit_code = 0;
}
