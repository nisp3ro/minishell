/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 08:56:25 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/02/03 09:05:53 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	should_continue_parsing(t_tokenizer **tok)
{
	if ((*tok)->full_cmd[(*tok)->i]
		&& (!is_delimiter((*tok)->full_cmd[(*tok)->i])
			&& !is_quote((*tok)->full_cmd[(*tok)->i]) && !(*tok)->in_here_doc))
		return (true);
	else if ((*tok)->full_cmd[(*tok)->i] && (*tok)->in_here_doc
		&& (!is_delimiter((*tok)->full_cmd[(*tok)->i])))
		return (true);
	else
		return (false);
}

void	tokenizer_error(t_data *data, t_tokenizer **tok, bool syntax_error)
{
	(*tok)->stop = true;
	if (syntax_error)
	{
		write(2, "minishell: syntax error\n", 24);
		data->g_exit_code = 2;
	}
}
