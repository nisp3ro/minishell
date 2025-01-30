/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 13:26:34 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/30 13:06:05 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	unset_from_envp(t_command *command, t_data *data)
{
	int	i;
	int	j;
	int	k;

	k = 0;
	if (!command->args[1])
		return ;
	while (command->args[++k])
	{
		i = -1;
		while (data->envp[++i] != NULL)
		{
			if (ft_strncmp(data->envp[i], command->args[k],
					ft_strlen(command->args[k])) == 0
				&& data->envp[i][ft_strlen(command->args[k])] == '=')
			{
				free(data->envp[i]);
				j = i - 1;
				while (data->envp[++j] != NULL)
					data->envp[j] = data->envp[j + 1];
				break ;
			}
		}
	}
	data->g_exit_code = 0;
}

void	unset_from_vars(char *arg, t_vars **vars)
{
	t_vars	*tmp;
	t_vars	*prev;

	tmp = *vars;
	prev = NULL;
	while (tmp)
	{
		if (ft_strncmp(tmp->name, arg, ft_strlen(arg)) == 0
			&& tmp->name[ft_strlen(arg)] == '\0')
		{
			if (prev)
				prev->next = tmp->next;
			else
				*vars = tmp->next;
			return (free(tmp->name), free(tmp->value), free(tmp));
		}
		prev = tmp;
		tmp = tmp->next;
	}
}

// For minishell tester
// if (ft_strcmp(command->args[1], "HOME") == 0) //WTF?
// 	return ;
void	ft_unset(t_command *command, t_data *data)
{
	int	i;

	i = 0;
	if (!command->args[1])
		return ;
	unset_from_envp(command, data);
	while (command->args[++i])
	{
		unset_from_vars(command->args[i], &data->vars);
		unset_from_vars(command->args[i], &data->exp_vars);
	}
	i = 0;
	while (command->args[++i])
	{
		if (ft_strncmp(command->args[i], "PWD", 3) == 0)
		{
			data->unset_pwd = true;
			break ;
		}
	}
	data->g_exit_code = 0;
}
