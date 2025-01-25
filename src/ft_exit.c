/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 13:29:06 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/25 14:08:59 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_exit(t_data *data, t_command *command)
{
	int	i;
	int	j;
	int	num;

	i = 0;
	j = 0;
	write(STDOUT_FILENO, "exit\n", 5);
	if (command->args[1] && command->args[2])
	{
		write(STDERR_FILENO, " too many arguments\n", 20);
		exit(1);
	}
	if (command->args[1])
	{
		while (command->args[1][i])
		{
			if (!ft_isdigit(command->args[1][i]) && command->args[1][i] != '-'
				&& command->args[1][i] != '+')
			{
				write(STDERR_FILENO, " numeric argument required\n", 27);
				g_exit_code = 2;
				exit(g_exit_code);
			}
			i++;
		}
		if ((ft_strlen(command->args[1]) == 19 && ft_strncmp(command->args[1], "9223372036854775807", 19) > 0)
			|| (ft_strlen(command->args[1]) == 20 && ft_strncmp(command->args[1], "-9223372036854775808", 20) > 0)
			|| ft_strlen(command->args[1]) > 21)
		{
			write(STDERR_FILENO, " numeric argument required\n", 27);
			g_exit_code = 2;
			exit(g_exit_code);
		}
		num = ft_atoi(command->args[1]);
		if (num < 0)
			num = 256 + num;
		g_exit_code = num;
	}
	clean_cmd(command);
	clean_data(data);
	exit(g_exit_code);
}
