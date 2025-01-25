/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 07:21:39 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/25 07:21:45 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	create_exp_vars(t_data *data, t_vars **exp_v, int flag)
{
	int		i;
	char	*var;
	char	*equal_sign;

	i = 0;
	while (data->envp[i + flag])
	{
		var = ft_strdup(data->envp[i]);
		if (var == NULL)
			return (ERROR);
		equal_sign = ft_strchr(var, '=');
		*equal_sign = '\0';
		equal_sign++;
		if (set_variable(exp_v, var, equal_sign) == ERROR)
		{
			free(var);
			clean_variables(*exp_v);
			return (ERROR);
		}
		free(var);
		i++;
	}
	return (OK);
}

int	handle_no_env(t_data *data)
{
	if (create_envp(data) == ERROR)
		return (ERROR);
	if (create_exp_vars(data, &data->exp_vars, 0))
		return (clean_mtx(data->envp), ERROR);
	return (OK);
}

int	init_data(t_data *data, char *envp[])
{
	data->exp_vars = NULL;
	if (envp == NULL || envp[0] == NULL || envp[0][0] == '\0')
		handle_no_env(data);
	else
	{
		data->envp = cpy_env(envp);
		if (data->envp == NULL)
			return (ERROR);
		if (update_envp(data) == ERROR)
			return (clean_mtx(data->envp), ERROR);
		if (create_exp_vars(data, &data->exp_vars, 1))
			return (clean_mtx(data->envp), ERROR);
	}
	sort_list(&data->exp_vars, data->exp_vars);
	data->home = NULL;
	data->pwd = getcwd(NULL, 0);
	if (!data->pwd)
		return (clean_mtx(data->envp), clean_variables(data->exp_vars), ERROR);
	data->oldpwd = getcwd(NULL, 0);
	if (!data->oldpwd)
		return (clean_mtx(data->envp), clean_variables(data->exp_vars),
			free(data->pwd), ERROR);
	data->prompt = NULL;
	data->vars = NULL;
	return (ft_recover_history(data), OK);
}
