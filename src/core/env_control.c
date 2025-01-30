/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_control.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 11:13:07 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/30 13:07:05 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	special_set_exp(t_data *data, char *name)
{
	int	i;

	i = 0;
	if (mini_getenv(name, data->envp) != NULL)
		return (OK);
	while (data->envp[i])
		i++;
	data->envp = ft_realloc(data->envp, sizeof(char *) * (i + 2));
	data->envp[i] = ft_strdup(name);
	if (data->envp[i] == NULL)
	{
		perror("malloc");
		exit(1);
	}
	data->envp[i + 1] = NULL;
	return (OK);
}
// OJO EXIT PORQUE SALIDA CRITICA

int	set_exp(t_data *data, char *name, char *value)
{
	int	i;

	i = 0;
	if (mini_getenv(name, data->envp) != NULL)
		return (OK);
	while (data->envp[i])
		i++;
	data->envp = ft_realloc(data->envp, sizeof(char *) * (i + 2));
	data->envp[i] = malloc(ft_strlen(name) + ft_strlen(value) + 2);
	if (data->envp[i] == NULL)
	{
		perror("malloc");
		exit(1);
	}
	ft_strcpy(data->envp[i], name);
	ft_strcat(data->envp[i], "=");
	ft_strcat(data->envp[i], value);
	data->envp[i + 1] = NULL;
	return (OK);
}
// OJO EXIT PORQUE SALIDA CRITICA

void	set_last_cmd_arg(t_data *data, char *name, char *value)
{
	int	i;

	i = 0;
	while (data->envp[i] != NULL)
	{
		if (ft_strncmp(data->envp[i], name, ft_strlen(name)) == 0
			&& data->envp[i][ft_strlen(name)] == '=')
		{
			free(data->envp[i]);
			data->envp[i] = malloc(ft_strlen(name) + ft_strlen(value) + 2);
			if (data->envp[i] == NULL)
			{
				perror("malloc");
				exit(1);
			}
			ft_strcpy(data->envp[i], name);
			ft_strcat(data->envp[i], "=");
			ft_strcat(data->envp[i], value);
			return ;
		}
		i++;
	}
	set_exp(data, name, value);
}
// OJO EXIT PORQUE SALIDA CRITICA
