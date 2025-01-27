/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 08:56:25 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/27 12:51:34 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	should_continue_parsing(t_tokenizer **tok)
{
	return ((*tok)->full_cmd[(*tok)->i]
		&& (!is_delimiter((*tok)->full_cmd[(*tok)->i])
			&& !is_quote((*tok)->full_cmd[(*tok)->i]) && !(*tok)->in_here_doc)
		|| ((*tok)->full_cmd[(*tok)->i]
			&& !is_delimiter((*tok)->full_cmd[(*tok)->i])
			&& (*tok)->in_here_doc));
}

void	tokenizer_error(t_tokenizer **tok, bool syntax_error)
{
	(*tok)->stop = true;
	if (syntax_error)
	{
		write(2, "minishell: syntax error\n", 24);
		g_exit_code = 2;
	}
}
