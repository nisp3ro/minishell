/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_vars.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 15:31:40 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/28 15:41:18 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	update_exp_var_value(t_vars *tmp_vars, char *key, char *value,
		t_data *data)
{
	free(tmp_vars->value);
	if (data->unset_pwd && ft_strncmp(key, "OLDPWD", 6) == 0)
		tmp_vars->value = ft_strdup("");
	else
		tmp_vars->value = ft_strdup(value);
	if (!tmp_vars->value)
	{
		perror("malloc");
		exit(1);
	}
}

static void	find_and_update_exp_var(t_data *data, char *key, char *value)
{
	t_vars	*tmp_vars;

	tmp_vars = data->exp_vars;
	while (tmp_vars)
	{
		if (ft_strncmp(tmp_vars->name, key, ft_strlen(key)) == 0)
		{
			update_exp_var_value(tmp_vars, key, value, data);
			break ;
		}
		tmp_vars = tmp_vars->next;
	}
}

static void	update_envp_var(t_data *data, int i, char *key, char *value)
{
	char	*tmp;

	tmp = NULL;
	free(data->envp[i]);
	if (data->unset_pwd && ft_strncmp(key, "OLDPWD", 6) == 0)
		data->envp[i] = ft_strdup(key);
	else
	{
		tmp = ft_strjoin(key, "=");
		if (!tmp)
		{
			perror("malloc");
			exit(1);
		}
		data->envp[i] = ft_strjoin(tmp, value);
		free(tmp);
		if (!data->envp[i])
		{
			perror("malloc");
			exit(1);
		}
	}
}

static void	find_and_update_envp(t_data *data, char *key,
		char *value)
{
	int	i;

	i = 0;
	while (data->envp[i])
	{
		if (ft_strncmp(data->envp[i], key, ft_strlen(key)) == 0)
		{
			update_envp_var(data, i, key, value);
			break ;
		}
		i++;
	}
}

void	update_env_var(t_data *data, char *key, char *value, char ***envp)
{
	find_and_update_exp_var(data, key, value);
	find_and_update_envp(data, key, value);
	if (data->unset_pwd)
		data->unset_pwd = false;
}
