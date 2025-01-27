/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_02.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 08:08:51 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/27 09:28:09 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_token	*process_token_value(t_tokenizer **tok, char *token_value)
{
	if (token_value && token_value[0])
	{
		return (add_token(tok, TOKEN_WORD, token_value));
	}
	return (NULL);
}

char	*handle_word(t_tokenizer **tok, t_data *data)
{
	char	*tmp;
	char	*tmp2;
	int		start;

	start = (*tok)->i;
	while (should_continue_parsing(tok))
		(*tok)->i++;
	tmp = ft_substr((*tok)->full_cmd, start, (*tok)->i - start);
	tmp2 = expand_variables(tmp, data->envp, data);
	free(tmp);
	if (!tmp2)
		return (tokenizer_error(tok, false), NULL);
	if (!(*tok)->token_value)
		(*tok)->token_value = tmp2;
	else
	{
		(*tok)->token_value = ft_strjoin((*tok)->token_value, tmp2);
		free(tmp2);
		if (!(*tok)->token_value)
			return (tokenizer_error(tok, false), NULL);
	}
	return ((*tok)->token_value);
}

char	*handle_quotes(t_tokenizer **tok, t_data *data, int start, char **tmp)
{
	while ((*tok)->full_cmd[(*tok)->i]
		&& (*tok)->full_cmd[(*tok)->i] != (*tok)->quote)
		(*tok)->i++;
	if (!(*tok)->full_cmd[(*tok)->i])
		return (tokenizer_error(tok, true), NULL);
	tmp[0] = ft_substr((*tok)->full_cmd, start, (*tok)->i - start);
	if ((*tok)->quote != '\'')
	{
		tmp[1] = expand_variables(tmp[0], data->envp, data);
		free(tmp[0]);
	}
	else
		tmp[1] = tmp[0];
	if (!tmp[1])
		return (tokenizer_error(tok, false), NULL);
	if (!(*tok)->token_value)
		(*tok)->token_value = tmp[1];
	else
	{
		(*tok)->token_value = ft_strjoin((*tok)->token_value, tmp[1]);
		free(tmp[1]);
		if (!(*tok)->token_value)
			return (tokenizer_error(tok, false), NULL);
	}
	return ((*tok)->token_value);
}

bool	manage_quotes(t_tokenizer **tok, char *full_cmd, t_data *data)
{
	char	*tmp[2];

	if (is_quote(full_cmd[(*tok)->i]) && !(*tok)->in_here_doc)
	{
		(*tok)->quote = full_cmd[(*tok)->i];
		(*tok)->i++;
		(*tok)->token_value = handle_quotes(tok, data, (*tok)->i, tmp);
		(*tok)->i++;
		(*tok)->quote = 0;
		return (OK);
	}
	return (ERROR);
}

t_token	*token_inner_loop(t_tokenizer **tok, t_data *data, t_token **current)
{
	while (1 && (*tok)->stop == false)
	{
		if (manage_quotes(tok, (*tok)->full_cmd, data) == OK)
			continue ;
		else if (!is_delimiter((*tok)->full_cmd[(*tok)->i]))
		{
			(*tok)->token_value = handle_word(tok, data);
			(*tok)->in_here_doc = false;
			if ((*tok)->full_cmd[(*tok)->i] == '\0')
			{
				*current = process_token_value(tok, (*tok)->token_value);
				(*tok)->token_value = NULL;
				break ;
			}
		}
		else
		{
			(*tok)->in_here_doc = false;
			*current = process_token_value(tok, (*tok)->token_value);
			(*tok)->token_value = NULL;
			break ;
		}
	}
	return (*current);
}
// OJO Comprobar free de linea 109 y 118 ¿?¿¿? nose si siempre hay que liberar
