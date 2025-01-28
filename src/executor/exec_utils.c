/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 16:57:57 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/28 17:57:09 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	init_search_command_vars(t_search_command *vars)
{
	vars->command_path = NULL;
	vars->i = 0;
}

char	*find_command_in_path(char *command, char **envp)
{
	t_search_command	vars;

	vars.path = mini_getenv("PATH", envp);
	if (!vars.path)
		return (NULL);
	vars.directories = ft_split(vars.path, ':');
	if (!vars.directories)
		return (NULL);
	init_search_command_vars(&vars);
	while (vars.directories[vars.i])
	{
		vars.command_path = ft_strjoin(vars.directories[vars.i], "/");
		if (!vars.command_path)
			return (clean_mtx(vars.directories), NULL);
		vars.tmp = ft_strjoin(vars.command_path, command);
		free(vars.command_path);
		vars.command_path = vars.tmp;
		if (access(vars.command_path, X_OK) == 0)
			return (clean_mtx(vars.directories), vars.command_path);
		free(vars.command_path);
		vars.command_path = NULL;
		vars.i++;
	}
	clean_mtx(vars.directories);
	return (NULL);
}

void	ft_create_custom_path(char **path, t_command *command)
{
	int	i;
	int	len;

	*path = command->args[0];
	len = ft_strlen(command->args[0]);
	i = len - 1;
	while (i >= 0 && command->args[0][i] != '/')
		i--;
	command->args[0] += i;
}

bool	check_cmd_args(t_command *command)
{
	if (command->args && command->args[0] != NULL
		&& command->args[0][0] != '\0')
		return (true);
	return (false);
}

void	init_pip(t_pip_vars *pip, t_command **command)
{
	pip->i = 0;
	pip->in_fd = STDIN_FILENO;
	pip->command_head = *command;
	wait_signal(0);
}
