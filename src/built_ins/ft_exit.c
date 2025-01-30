/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 13:29:06 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/30 13:05:02 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	check_invalid_arguments(t_command *command)
{
	if (command->args[1] && command->args[2])
	{
		write(STDERR_FILENO, " too many arguments\n", 20);
		exit(1);
	}
}

static void	validate_numeric_argument(t_data *data, t_command *command, int i)
{
	while (command->args[1][i])
	{
		if (!ft_isdigit(command->args[1][i]) && command->args[1][i] != '-'
			&& command->args[1][i] != '+')
		{
			write(STDERR_FILENO, " numeric argument required\n", 27);
			data->g_exit_code = 2;
			exit(data->g_exit_code);
		}
		i++;
	}
}

static void	check_numeric_limits(t_data *data, t_command *command)
{
	if ((ft_strlen(command->args[1]) == 19 && ft_strncmp(command->args[1],
				"9223372036854775807", 19) > 0)
		|| (ft_strlen(command->args[1]) == 20 && ft_strncmp(command->args[1],
				"-9223372036854775808", 20) > 0)
		|| ft_strlen(command->args[1]) > 21)
	{
		write(STDERR_FILENO, " numeric argument required\n", 27);
		data->g_exit_code = 2;
		exit(data->g_exit_code);
	}
}

static void	set_exit_code(t_data *data, t_command *command)
{
	int	num;

	num = ft_atoi(command->args[1]);
	if (num < 0)
		num = 256 + num;
	data->g_exit_code = num;
}

void	ft_exit(t_data *data, t_command *command)
{
	int	i;

	i = 0;
	write(STDOUT_FILENO, "exit\n", 5);
	check_invalid_arguments(command);
	if (command->args[1])
	{
		validate_numeric_argument(data, command, i);
		check_numeric_limits(data, command);
		set_exit_code(data, command);
	}
	rl_clear_history();
	clean_cmd(command);
	clean_data(data);
	exit(data->g_exit_code);
}
