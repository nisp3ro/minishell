#include "../include/minishell.h"

int	is_delimiter(char c)
{
	return (c == ' ' || c == '|' || c == '<' || c == '>');
}

bool	is_quote(char c)
{
	if (c == '\'' || c == '\"')
		return (true);
	return (false);
}

t_token	*add_token(t_token **tokens, t_token_type type, char *value)
{
	t_token	*new_token;
	t_token	*last;

	new_token = malloc(sizeof(t_token));
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

t_token	*tokenize(char *full_cmd, char **envp, t_data *data)
{
	t_token	*tokens;
	t_token	*current;
	int		i;
	int		start;
	int		tokeind;
	char	quote;
	char	*token_value;
	char	*tmp;
	char	*tmp2;
	bool	in_here_doc;

	tokens = NULL;
	current = NULL;
	i = 0;
	start = 0;
	tokeind = 0;
	quote = 0;
	token_value = NULL;
	tmp = NULL;
	tmp2 = NULL;
	in_here_doc = false;
	while (full_cmd[i])
	{
		if (is_delimiter(full_cmd[i]) && !quote)
		{
			if (full_cmd[i] == '|')
			{
				in_here_doc = false;
				current = add_token(&tokens, TOKEN_PIPE, "|");
			}
			else if (full_cmd[i] == '<' && full_cmd[i + 1] == '<')
			{
				current = add_token(&tokens, TOKEN_HEREDOC, "<<");
				i++;
				in_here_doc = true;
			}
			else if (full_cmd[i] == '<')
			{
				in_here_doc = false;
				current = add_token(&tokens, TOKEN_REDIRECT_IN, "<");
			}
			else if (full_cmd[i] == '>' && full_cmd[i + 1] == '>')
			{
				in_here_doc = false;
				current = add_token(&tokens, TOKEN_APPEND_OUT, ">>");
				i++;
			}
			else if (full_cmd[i] == '>')
			{
				in_here_doc = false;
				current = add_token(&tokens, TOKEN_REDIRECT_OUT, ">");
			}
			i++;
		}
		else
		{
			while (1)
			{
				if (is_quote(full_cmd[i]) && in_here_doc == false)
				{
					quote = full_cmd[i];
					i++;
					start = i;
					while (full_cmd[i] && full_cmd[i] != quote)
					{
						i++;
					}
					if (!full_cmd[i])
					{
						printf("minishell: syntax error\n");
						exit(0);
					}
					tmp = ft_substr(full_cmd, start, i - start);
					if (quote != '\'')
					{
						tmp = expand_variables(tmp, envp, data);
					}
					if (!token_value)
						token_value = tmp;
					else
						token_value = ft_strjoin(token_value, tmp);
					quote = 0;
					i++;
				}
				else if (!is_delimiter(full_cmd[i]))
				{
					start = i;
					while (full_cmd[i] && !is_delimiter(full_cmd[i]) && !is_quote(full_cmd[i]) && in_here_doc == false || (full_cmd[i] && !is_delimiter(full_cmd[i]) && in_here_doc == true))
					{
						i++;
					}
					in_here_doc = false;
					tmp = ft_substr(full_cmd, start, i - start);
					tmp = expand_variables(tmp, envp, data);
					if (!token_value)
						token_value = tmp;
					else
						token_value = ft_strjoin(token_value, tmp);
					if (full_cmd[i] == '\0')
					{
						current = add_token(&tokens, TOKEN_WORD, token_value);
						token_value = NULL;
						break ;
					}
				}
				else
				{
					in_here_doc = false;
					current = add_token(&tokens, TOKEN_WORD, token_value);
					token_value = NULL;
					break ;
				}
			}
		}
	}
	return (tokens);
}
