/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 13:10:06 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/23 19:03:06 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
		printf("minishell: syntax error near unexpected token `|'\n");
		g_exit_code = 258;
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

int	interactive_mode(t_data *data, char *envp[])
{
	char	*line[2];
	int		i;

	while (1)
	{
		i = 0;
		if (get_prompt(&data->prompt, data) == ERROR)
			return (ERROR);
		line[0] = readline(data->prompt);
		if (!line[0])
			return (printf("exit\n"), g_exit_code);
		while (isspace((unsigned char)line[0][i]))
			i++;
		if (check_cmd_start(line[0], i) == ERROR)
			continue ;
		line[0] = unfinished_pipe(line[0]);
		if (line[0] == NULL)
			continue ;
		line[1] = clean_line(line[0], data);
		if (line[1] == NULL)
			continue ;
		else if (line[1][0] != '\0')
			token_parsec_exec(line[1], data);
	}
	return (g_exit_code);
}
