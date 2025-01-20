/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 13:26:02 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/20 20:06:12 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_env(t_data *data)
{
	int	i;

	i = 0;
	while (data->envp[i])
	{
		write(STDOUT_FILENO, data->envp[i], ft_strlen(data->envp[i]));
		write(STDOUT_FILENO, "\n", 1);
		i++;
	}
	g_exit_code = 0;
}
