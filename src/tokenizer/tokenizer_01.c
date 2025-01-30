/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_01.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 08:04:07 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/30 13:31:32 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_token	*add_token(t_data *data, t_tokenizer **tok, t_token_type type,
		char *value)
{
	t_token	*new_token;
	t_token	*last;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (tokenizer_error(data, tok, false), NULL);
	last = (*tok)->tokens;
	new_token->type = type;
	new_token->value = value;
	if (!new_token->value)
		return (free(new_token), tokenizer_error(data, tok, false), NULL);
	new_token->next = NULL;
	if (!last)
	{
		(*tok)->tokens = new_token;
		return (new_token);
	}
	while (last->next)
		last = last->next;
	last->next = new_token;
	return (new_token);
}

t_token	*check_hd_and_create_tok(t_data *data, t_tokenizer **tok,
		enum e_token_type type, bool hd_val)
{
	t_token	*current;
	char	*value;

	value = NULL;
	(*tok)->in_here_doc = hd_val;
	if (type == TOKEN_PIPE)
		value = "|";
	else if (type == TOKEN_HEREDOC)
		value = "<<";
	else if (type == TOKEN_REDIRECT_IN)
		value = "<";
	else if (type == TOKEN_APPEND_OUT)
		value = ">>";
	else if (type == TOKEN_REDIRECT_OUT)
		value = ">";
	current = add_token(data, tok, type, value);
	return (current);
}

t_token	*handle_delimiter(t_data *data, t_tokenizer **tok)
{
	t_token	*cur;

	cur = NULL;
	if ((*tok)->full_cmd[(*tok)->i] == '|')
		cur = check_hd_and_create_tok(data, tok, TOKEN_PIPE, false);
	else if ((*tok)->full_cmd[(*tok)->i] == '<' && (*tok)->full_cmd[(*tok)->i
			+ 1] == '<')
	{
		cur = check_hd_and_create_tok(data, tok, TOKEN_HEREDOC, true);
		(*tok)->i++;
	}
	else if ((*tok)->full_cmd[(*tok)->i] == '<')
		cur = check_hd_and_create_tok(data, tok, TOKEN_REDIRECT_IN, false);
	else if ((*tok)->full_cmd[(*tok)->i] == '>' && (*tok)->full_cmd[(*tok)->i
			+ 1] == '>')
	{
		cur = check_hd_and_create_tok(data, tok, TOKEN_APPEND_OUT, false);
		(*tok)->i++;
	}
	else if ((*tok)->full_cmd[(*tok)->i] == '>')
		cur = check_hd_and_create_tok(data, tok, TOKEN_REDIRECT_OUT, false);
	(*tok)->i++;
	return (cur);
}

void	init_tokenizer(t_tokenizer **tokenizer,
					t_token **current,
					char *full_cmd)
{
	t_token	*tokens;

	tokens = NULL;
	(*tokenizer)->i = 0;
	(*tokenizer)->quote = 0;
	(*tokenizer)->full_cmd = full_cmd;
	(*tokenizer)->token_value = NULL;
	(*tokenizer)->in_here_doc = false;
	(*tokenizer)->tokens = tokens;
	(*tokenizer)->stop = false;
	*current = NULL;
}

t_token	*tokenize(char *full_cmd, t_data *data)
{
	t_token		*current;
	t_tokenizer	*tok;
	t_token		*tokens;

	tok = malloc(sizeof(t_tokenizer));
	if (!tok)
		return (NULL);
	init_tokenizer(&tok, &current, full_cmd);
	while (full_cmd[tok->i])
	{
		if (is_delimiter(full_cmd[tok->i]) && !tok->quote)
			current = handle_delimiter(data, &tok);
		else
			current = token_inner_loop(&tok, data, &current);
		if (tok->stop == true)
			return (free_tokens(tok->tokens), NULL);
	}
	tokens = tok->tokens;
	free(tok);
	return (tokens);
}
