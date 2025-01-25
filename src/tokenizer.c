/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 13:20:49 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/24 16:35:05 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_token	*add_token(t_token **tokens, t_token_type type, char *value)
{
	t_token	*new_token;
	t_token	*last;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	last = *tokens;
	new_token->type = type;
	new_token->value = value;
	new_token->next = NULL;
	if (!last)
	{
		*tokens = new_token;
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
	current = add_token((*tok)->tokens, type, value);
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

char	*handle_quotes(t_tokenizer **tok, t_data *data)
{
	char	*tmp;
	int		start;

	(*tok)->i++;
	start = (*tok)->i;
	while ((*tok)->full_cmd[(*tok)->i]
		&& (*tok)->full_cmd[(*tok)->i] != (*tok)->quote)
		(*tok)->i++;
	if (!(*tok)->full_cmd[(*tok)->i])
	{
		printf("minishell: syntax error\n"); //OJO GESTIONAR SYNTAX ERROR
		exit(0);
	}
	tmp = ft_substr((*tok)->full_cmd, start, (*tok)->i - start);
	if ((*tok)->quote != '\'')
		tmp = expand_variables(tmp, data->envp, data);
	if (!(*tok)->token_value)
		(*tok)->token_value = tmp;
	else
		(*tok)->token_value = ft_strjoin((*tok)->token_value, tmp);
	(*tok)->i++;
	return ((*tok)->token_value);
}

char	*handle_word(t_tokenizer **tok, t_data *data)
{
	char	*tmp;
	int		start;

	start = (*tok)->i;
	while ((*tok)->full_cmd[(*tok)->i]
		&& (!is_delimiter((*tok)->full_cmd[(*tok)->i])
			&& !is_quote((*tok)->full_cmd[(*tok)->i]) && !(*tok)->in_here_doc)
		||
			((*tok)->full_cmd[(*tok)->i]
					&& !is_delimiter((*tok)->full_cmd[(*tok)->i])
					&& (*tok)->in_here_doc))
	{
		(*tok)->i++;
	}
	tmp = ft_substr((*tok)->full_cmd, start, (*tok)->i - start);
	tmp = expand_variables(tmp, data->envp, data);
	if (!(*tok)->token_value)
		(*tok)->token_value = tmp;
	else
		(*tok)->token_value = ft_strjoin((*tok)->token_value, tmp);
	return ((*tok)->token_value);
}

t_token	*process_token_value(t_token **tokens, char *token_value)
{
	if (token_value && token_value[0])
	{
		return (add_token(tokens, TOKEN_WORD, token_value));
	}
	return (NULL);
}

void	init_tokenizer(t_tokenizer **tokenizer, t_token **tokens,
		t_token **current, char *full_cmd)
{
	(*tokenizer)->i = 0;
	(*tokenizer)->quote = 0;
	(*tokenizer)->full_cmd = full_cmd;
	(*tokenizer)->token_value = NULL;
	(*tokenizer)->in_here_doc = false;
	(*tokenizer)->tokens = tokens;
	*tokens = NULL;
	*current = NULL;
}

bool	manage_quotes(t_tokenizer **tok, char *full_cmd, t_data *data)
{
	if (is_quote(full_cmd[(*tok)->i]) && !(*tok)->in_here_doc)
	{
		(*tok)->quote = full_cmd[(*tok)->i];
		(*tok)->token_value = handle_quotes(tok, data);
		(*tok)->quote = 0;
		return (OK);
	}
	return (ERROR);
}

t_token	*process_inner_loop(t_tokenizer **tok, t_data *data, t_token **current)
{
	while (1)
	{
		if (manage_quotes(tok, (*tok)->full_cmd, data) == OK)
			continue ;
		else if (!is_delimiter((*tok)->full_cmd[(*tok)->i]))
		{
			(*tok)->token_value = handle_word(tok, data);
			(*tok)->in_here_doc = false;
			if ((*tok)->full_cmd[(*tok)->i] == '\0')
			{
				*current = process_token_value((*tok)->tokens,
						(*tok)->token_value);
				(*tok)->token_value = NULL;
				break ;
			}
		}
		else
		{
			(*tok)->in_here_doc = false;
			*current = process_token_value((*tok)->tokens, (*tok)->token_value);
			(*tok)->token_value = NULL;
			break ;
		}
	}
	return (*current);
}

t_token	*tokenize(char *full_cmd, t_data *data)
{
	t_token		*tokens;
	t_token		*current;
	t_tokenizer	*tok;

	tok = malloc(sizeof(t_tokenizer));
	init_tokenizer(&tok, &tokens, &current, full_cmd);
	while (full_cmd[tok->i])
	{
		if (is_delimiter(full_cmd[tok->i]) && !tok->quote)
			current = handle_delimiter(&tok);
		else
			current = process_inner_loop(&tok, data, &current);
	}
	return (tokens);
}
