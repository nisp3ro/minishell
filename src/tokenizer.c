#include "../include/minishell.h"

int	is_delimiter(char c)
{
	return (c == ' ' || c == '|' || c == '<' || c == '>');
}

int	is_quote(char c)
{
	return (c == '\'' || c == '"');
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
	t_token *tokens = NULL;
	t_token *current = NULL;
	int i = 0;
	int tokeind = 0;
	char quote = 0;

	while (full_cmd[i])
	{
		if (is_delimiter(full_cmd[i]) && !quote)
		{
			if (full_cmd[i] == '|')
				current = add_token(&tokens, TOKEN_PIPE, "|");
			else if (full_cmd[i] == '<' && full_cmd[i + 1] == '<')
			{
				current = add_token(&tokens, TOKEN_HEREDOC, "<<");
				i++;
			}
			else if (full_cmd[i] == '<')
				current = add_token(&tokens, TOKEN_REDIRECT_IN, "<");
			else if (full_cmd[i] == '>' && full_cmd[i + 1] == '>')
			{
				current = add_token(&tokens, TOKEN_APPEND_OUT, ">>");
				i++;
			}
			else if (full_cmd[i] == '>')
				current = add_token(&tokens, TOKEN_REDIRECT_OUT, ">");
		}
		else if (is_quote(full_cmd[i]))
		{
			// Se detecta una comilla simple o doble
			quote = full_cmd[i];
			int start = i + 1; // Ignorar la comilla de apertura
			i++;

			// Buscar la comilla de cierre correspondiente
			while (full_cmd[i] && full_cmd[i] != quote)
				i++;

			// Si se encuentra la comilla de cierre, crear un token
			if (full_cmd[i] == quote)
			{
				char *token_value = ft_substr(full_cmd, start, i - start);
				if (quote != '\'')
					token_value = expand_variables(token_value, envp, data);
				current = add_token(&tokens, TOKEN_WORD, token_value);
			}
			i++; // Avanzamos para saltar la comilla de cierre
		}
		else if (!is_delimiter(full_cmd[i]))
		{
			// Palabra normal, fuera de comillas
			int start = i;
			while (full_cmd[i] && !is_delimiter(full_cmd[i])
				&& !is_quote(full_cmd[i]))
				i++;
			char *token_value = ft_substr(full_cmd, start, i - start);

			// Expansión de variables si no estamos dentro de comillas simples
			if (quote != '\'')
			{
				token_value = expand_variables(token_value, envp, data);
			}

			current = add_token(&tokens, TOKEN_WORD, token_value);
			i--; // Retrocedemos 1 porque el bucle principal ya avanza i
		}
		i++;
	}
	return (tokens);
}