/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 13:26:34 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/20 13:58:59 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	unset_from_envp(t_command *command, t_data *data)
{
	int	i;
	int	j;

	i = 0;
	if (!command->args[1])
		return ;
	while (data->envp[i] != NULL)
	{
		if (ft_strncmp(data->envp[i], command->args[1],
				ft_strlen(command->args[1])) == 0
			&& data->envp[i][ft_strlen(command->args[1])] == '=')
		{
			free(data->envp[i]);
			j = i;
			while (data->envp[j] != NULL)
			{
				data->envp[j] = data->envp[j + 1];
				j++;
			}
			break ;
		}
		i++;
	}
	g_error = 0;
}

void	unset_from_vars(t_command *command, t_vars **vars)
{
	t_vars	*tmp;
	t_vars	*prev;

	tmp = *vars;
	prev = NULL;
	while (tmp)
	{
		if (ft_strncmp(tmp->name, command->args[1],
				ft_strlen(command->args[1])) == 0
			&& tmp->name[ft_strlen(command->args[1])] == '\0')
		{
			if (prev)
				prev->next = tmp->next;
			else
				*vars = tmp->next;
			free(tmp->name);
			free(tmp->value);
			free(tmp);
			return ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}

void	ft_unset(t_command *command, t_data *data)
{
	int	i;
	int	j;

	i = 0;
	if (!command->args[1])
		return ;
	// if (ft_strcmp(command->args[1], "HOME") == 0) //WTF?
	// 	return ;
	unset_from_envp(command, data);
	unset_from_vars(command, &data->vars);
	unset_from_vars(command, &data->exp_vars);
	g_error = 0;
}
