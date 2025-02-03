/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins_prepipe.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 09:51:02 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/02/03 11:22:13 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static bool	handle_builtins_with_redirs(t_command *command, t_data *data)
{
	if (ft_strncmp(command->args[0], "exit", 5) == 0)
	{
		if (check_redirs(command) == true)
			return (ft_exit(data, command), true);
		return (true);
	}
	if (ft_strncmp(command->args[0], "cd", 3) == 0)
	{
		if (check_redirs(command) == true)
			return (ft_cd(command, data), true);
		return (true);
	}
	if (ft_strncmp(command->args[0], "export", 7) == 0)
	{
		if (check_redirs(command) == true)
			return (ft_export(command, data), true);
		return (true);
	}
	if (ft_strncmp(command->args[0], "unset", 6) == 0)
	{
		if (check_redirs(command) == true)
			return (ft_unset(command, data), true);
		return (true);
	}
	return (false);
}

static bool	handle_history_builtins(t_command *command, t_data *data)
{
	if (ft_strncmp(command->args[0], "history", 8) == 0 && !command->args[1])
		return (print_history(data), true);
	if (ft_strncmp(command->args[0], "history", 8) == 0
		&& ft_strncmp(command->args[1], "-c", 2) == 0
		&& command->args[1][2] != 'w')
		return (rl_clear_history(), true);
	if (ft_strncmp(command->args[0], "history", 8) == 0
		&& ft_strncmp(command->args[1], "-cw", 4) == 0)
		return (delete_history_file(data), true);
	return (false);
}

bool	check_builtin_prepipe(t_command *command, t_data *data)
{
	if (!command->args)
		return (false);
	if (handle_builtins_with_redirs(command, data))
		return (true);
	if (handle_history_builtins(command, data))
		return (true);
	return (false);
}
