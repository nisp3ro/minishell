/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 16:42:19 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/17 16:46:04 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	**cpy_env(char *envp[])
{
	char	**cpy;
	int		i;

	i = 0;
	while (envp[i])
		i++;
	cpy = malloc(sizeof(char *) * (i + 1));
	if (cpy == NULL)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		cpy[i] = ft_strdup(envp[i]);
		if (!cpy[i])
		{
			while (i >= 0)
				free(cpy[i--]);
			free(cpy);
			return (NULL);
		}
		i++;
	}
	cpy[i] = NULL;
	return (cpy);
}

int	create_envp(t_data *data)
{
	char	*tmp_pwd;
	char	*tmp_cwd;

	data->envp = malloc(sizeof(char *) * 4);
	if (data->envp == NULL)
		return (clean_envp(data->envp), ERROR);
	tmp_pwd = ft_strdup("PWD=");
	if (tmp_pwd == NULL)
		return (clean_envp(data->envp), ERROR);
	tmp_cwd = getcwd(NULL, 0);
	if (tmp_cwd == NULL)
		return (free(tmp_pwd), clean_envp(data->envp), ERROR);
	data->envp[0] = ft_strjoin(tmp_pwd, tmp_cwd);
	free(tmp_pwd);
	free(tmp_cwd);
	if (data->envp[0] == NULL)
		return (clean_envp(data->envp), ERROR);
	data->envp[1] = ft_strdup("SHLVL= 0");
	if (data->envp[1] == NULL)
		return (clean_envp(data->envp), ERROR);
	data->envp[2] = ft_strdup("_=]");
	if (data->envp[2] == NULL)
		return (clean_envp(data->envp), ERROR);
	data->envp[3] = NULL;
	return (OK);
}

int	create_exp_vars(t_data *data, t_vars **exp_vars, int flag)
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
		if (set_variable(exp_vars, var, equal_sign) == ERROR)
		{
			free(var);
			clean_variables(*exp_vars);
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
		return (clean_envp(data->envp), ERROR);
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
		if (create_exp_vars(data, &data->exp_vars, 1))
			return (clean_envp(data->envp), ERROR);
	}
	sort_list(&data->exp_vars, data->exp_vars);
	data->home = NULL;
	data->pwd = getcwd(NULL, 0);
	if (!data->pwd)
		return (clean_envp(data->envp), clean_variables(data->exp_vars), ERROR);
	data->oldpwd = getcwd(NULL, 0);
	if (!data->oldpwd)
		return (clean_envp(data->envp), clean_variables(data->exp_vars),
			free(data->pwd), ERROR);
	data->prompt = NULL;
	data->vars = NULL;
	return (OK);
}
