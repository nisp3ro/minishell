/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_tools_01.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 17:28:52 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/28 17:29:29 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
