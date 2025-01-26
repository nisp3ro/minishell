/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 16:43:15 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/26 13:02:32 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void clean_data(t_data *data)
{
	clean_variables(data->vars);
	clean_variables(data->exp_vars);
	clean_mtx(data->envp);
	free(data->pwd);
	free(data->oldpwd);
}

void   clean_redir_list(t_redir **redir)
{
    t_redir *tmp;

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
			free(tmp->eof);
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

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		if (tmp->type == TOKEN_WORD)
			free(tmp->value);
		free(tmp);
	}
}

void	clean_variables(t_vars *vars)
{
	t_vars	*tmp;

	while (vars)
	{
		tmp = vars;
		vars = vars->next;
		free(tmp->name);
		free(tmp->value);
		free(tmp);
	}
}

void	clean_mtx(char **mtx)
{
	int	i;

	i = 0;
	while (mtx && mtx[i])
	{
		free(mtx[i]);
		i++;
	}
	if (mtx)
		free(mtx);
}

char	*clean_line(char *line, t_data *data)
{
	char	*full_cmd;

	if (line[0] != '\0' && !is_all_spaces(line))
		ft_write_history(data, line);
	full_cmd = ft_strtrim(line, " ");
	free(line);
	return (full_cmd);
}
