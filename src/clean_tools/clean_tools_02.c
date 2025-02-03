/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_tools_02.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 17:28:54 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/02/03 11:03:10 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	clean_data(t_data *data)
{
	clean_variables(data->vars);
	clean_variables(data->exp_vars);
	clean_mtx(data->envp);
	free(data->pwd);
	free(data->oldpwd);
}

void	clean_redir_list(t_redir **redir)
{
	t_redir	*tmp;

	while (*redir)
	{
		tmp = *redir;
		*redir = (*redir)->next;
		free(tmp->value);
		free(tmp);
	}
}

void	clean_cmd(t_command *command)
{
	t_command	*tmp;
	t_redir		*redir_tmp;

	while (command)
	{
		tmp = command;
		command = command->next;
		if (tmp->args)
			clean_mtx(tmp->args);
		if (tmp->eof)
			clean_mtx(tmp->eof);
		while (tmp->redir)
		{
			redir_tmp = tmp->redir;
			tmp->redir = tmp->redir->next;
			free(redir_tmp->value);
			free(redir_tmp);
		}
		free(tmp);
	}
}
