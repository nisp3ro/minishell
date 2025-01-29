/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 13:24:46 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/27 10:01:06 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

bool	check_builtin(t_command *command, t_data *data)
{
	if (!command->args)
		return (false);
	if (ft_strncmp(command->args[0], "exit", 5) == 0)
		return (ft_exit(data, command), true);
	if (ft_strncmp(command->args[0], "echo", 5) == 0)
		return (ft_echo(command), true);
	if (ft_strncmp(command->args[0], "cd", 3) == 0)
		return (ft_cd(command, data), true);
	if (ft_strncmp(command->args[0], "pwd", 4) == 0)
		return (ft_pwd(data), true);
	if (ft_strncmp(command->args[0], "export", 7) == 0)
		return (ft_export(command, data), true);
	if (ft_strncmp(command->args[0], "unset", 6) == 0)
		return (ft_unset(command, data), true);
	if (ft_strncmp(command->args[0], "env", 4) == 0)
		return (ft_env(data), true);
	if (ft_strncmp(command->args[0], "history", 8) == 0 && !command->args[1])
		return (print_history(data), true);
	if (ft_strncmp(command->args[0], "history", 8) == 0
		&& ft_strncmp(command->args[1], "-c", 3) == 0)
		return (rl_clear_history(), true);
	if (ft_strncmp(command->args[0], "history", 8) == 0
		&& ft_strncmp(command->args[1], "-cw", 4) == 0)
		return (delete_history_file(data), true);
	return (false);
}
