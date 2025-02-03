/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 13:25:38 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/02/03 08:52:32 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_echo(t_data *data, t_command *command)
{
	int	i;

	i = 1;
	while (command->args[i] != NULL && ft_strncmp("-n", command->args[i],
			3) == 0)
		i++;
	write(STDOUT_FILENO, command->args[i], ft_strlen(command->args[i]));
	while (command->args[++i])
	{
		write(STDOUT_FILENO, " ", 1);
		write(STDOUT_FILENO, command->args[i], ft_strlen(command->args[i]));
	}
	if (command->args[1] == NULL || (command->args[1] != NULL
			&& ft_strncmp("-n", command->args[1], 3)))
		write(STDOUT_FILENO, "\n", 1);
	data->g_exit_code = 0;
}
