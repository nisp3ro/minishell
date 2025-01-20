/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 13:29:06 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/20 14:56:28 by mrubal-c         ###   ########.fr       */
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
				exit(2);
			}
			i++;
		}
		num = ft_atoi(command->args[1]);
		while (num < 0)
			num += 256;
		g_error = num;
	}
	else
		g_error = 0;
	exit(g_error); // OJO! limPiar!!!!
}
