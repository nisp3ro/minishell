/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 13:25:38 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/28 15:59:18 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_echo(t_command *command)
{
	int	i;

	i = 0;
	if (command->args[1] != NULL && ft_strncmp("-n", command->args[1], 3) == 0)
		i++;
	i++;
	write(STDOUT_FILENO, command->args[i], ft_strlen(command->args[i]));
	while (command->args[++i])
	{
		write(STDOUT_FILENO, " ", 1);
		write(STDOUT_FILENO, command->args[i], ft_strlen(command->args[i]));
	}
	if (command->args[1] != NULL && ft_strncmp("-n", command->args[1], 3))
		write(STDOUT_FILENO, "\n", 1);
	g_exit_code = 0;
}
