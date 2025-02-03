/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 16:57:57 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/02/03 08:56:45 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	init_search_command_vars(t_search_command *vars)
{
	vars->command_path = NULL;
	vars->i = 0;
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
