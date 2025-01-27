/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 09:06:04 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/27 12:51:07 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	parse_last_cmd_arg(t_command *commands, t_data *data)
{
	int	i;

	if (ft_strncmp(commands->args[0], "export", 7) == 0
		&& commands->args[1] == NULL)
		set_last_cmd_arg(data, "_", commands->args[0]);
	else if (ft_strncmp(commands->args[0], "export", 7) != 0)
	{
		i = 0;
		while (commands->args[i])
			i++;
		set_last_cmd_arg(data, "_", commands->args[i - 1]);
	}
}

int	check_cmd_start(char *line, int i)
{
	if (line[i] == '|')
	{
		write(STDERR_FILENO,
			"minishell: syntax error near unexpected token `|'\n", 52);
		g_exit_code = 2;
		free(line);
		return (ERROR);
	}
	return (OK);
}

void	token_parsec_exec(char *full_cmd, t_data *data)
{
	t_token		*tokens;
	t_command	*commands;

	tokens = tokenize(full_cmd, data);
	free(full_cmd);
	if (tokens == NULL)
		return ;
	commands = parse_pipeline(data, tokens);
	free_tokens(tokens);
	if (commands == NULL)
		return ;
	if (commands->args && !commands->next)
		parse_last_cmd_arg(commands, data);
	if (!commands || !commands->next && check_builtin_prepipe(commands,
			data) == true)
		return ;
	else
		execute_pipeline(commands, data, data->envp);
}

char	*unfinished_pipe(char *line)
{
	char	*tmp[2];
	int		i;

	while (1)
	{
		i = ft_strlen(line) - 1;
		while (isspace((unsigned char)line[i]))
			i--;
		if (line[i] == '|')
		{
			tmp[0] = readline(">");
			tmp[1] = ft_strjoin(line, " ");
			if (tmp[1] == NULL)
				return (free(tmp[0]), free(line), NULL);
			free(line);
			line = ft_strjoin(tmp[1], tmp[0]);
			if (line == NULL)
				return (free(tmp[0]), free(tmp[1]), NULL);
			free(tmp[0]);
			free(tmp[1]);
		}
		else
			break ;
	}
	return (line);
}
