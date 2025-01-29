/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_01.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 08:04:07 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/27 12:53:53 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_token	*add_token(t_tokenizer **tok, t_token_type type, char *value)
{
	t_token	*new_token;
	t_token	*last;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (tokenizer_error(tok, false), NULL);
	last = (*tok)->tokens;
	new_token->type = type;
	new_token->value = value;
	if (!new_token->value)
		return (free(new_token), tokenizer_error(tok, false), NULL);
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

t_token	*check_hd_and_create_tok(t_tokenizer **tok, enum e_token_type type,
		char *value, bool hd_val)
{
	t_token	*current;

	(*tok)->in_here_doc = hd_val;
	current = add_token(tok, type, value);
	return (current);
}

t_token	*handle_delimiter(t_tokenizer **tok)
{
	t_token	*current;

	current = NULL;
	if ((*tok)->full_cmd[(*tok)->i] == '|')
		current = check_hd_and_create_tok(tok, TOKEN_PIPE, "|", false);
	else if ((*tok)->full_cmd[(*tok)->i] == '<' && (*tok)->full_cmd[(*tok)->i
			+ 1] == '<')
	{
		current = check_hd_and_create_tok(tok, TOKEN_HEREDOC, "<<", true);
		(*tok)->i++;
	}
	else if ((*tok)->full_cmd[(*tok)->i] == '<')
		current = check_hd_and_create_tok(tok, TOKEN_REDIRECT_IN, "<", false);
	else if ((*tok)->full_cmd[(*tok)->i] == '>' && (*tok)->full_cmd[(*tok)->i
			+ 1] == '>')
	{
		current = check_hd_and_create_tok(tok, TOKEN_APPEND_OUT, ">>", false);
		(*tok)->i++;
	}
	else if ((*tok)->full_cmd[(*tok)->i] == '>')
		current = check_hd_and_create_tok(tok, TOKEN_REDIRECT_OUT, ">", false);
	(*tok)->i++;
	return (current);
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
			current = handle_delimiter(&tok);
		else
			current = token_inner_loop(&tok, data, &current);
		if (tok->stop == true)
			return (free_tokens(tok->tokens), NULL);
	}
	tokens = tok->tokens;
	free(tok);
	return (tokens);
}
