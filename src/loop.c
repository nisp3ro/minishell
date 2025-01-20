/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 13:10:06 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/20 13:10:21 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	interactive_mode(t_data *data, char *envp[])
{
	char		*line;
	char		*full_cmd;
	t_token		*tokens;
	t_command	*commands;
	char		*tmp;
	char		*tmp2;
	int			i;

	i = 0;
	while (1)
	{
		//if (isatty(STDIN_FILENO))
		if (get_prompt(&data->prompt, data) == ERROR)
			return (ERROR);
		line = readline(data->prompt);
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		// else
		// {
		// 	line = readline("");
		// 	write(STDOUT_FILENO, "\033[A\r", 4);
		// 	if (!line)
		// 		break ;
		// }
		while (isspace((unsigned char)line[i]))
			i++;
		if (line[i] == '|')
		{
			printf("minishell: syntax error near unexpected token `|'\n");
			g_error = 258;
			free(line);
			continue ;
		}
		while (1)
		{
			i = ft_strlen(line) - 1;
			while (isspace((unsigned char)line[i]))
				i--;
			if (line[i] == '|')
			{
				tmp = readline(">");
				tmp2 = ft_strjoin(line, " ");
				free(line);
				line = ft_strjoin(tmp2, tmp);
				free(tmp);
				free(tmp2);
			}
			else
				break ;
		}
		if (line[0] != '\0' && !is_all_spaces(line))
			ft_write_history(data, line);
		full_cmd = ft_strtrim(line, " ");
		free(line);
		if (full_cmd == NULL)
			return (perror("Error"), ERROR);
		else if (full_cmd[0] != '\0')
		{
			tokens = tokenize(full_cmd, data);
			free(full_cmd);
			if (tokens == NULL)
				continue ;
			commands = parse_pipeline(data, tokens);
			free_tokens(tokens);
			if (commands == NULL)
				continue ;
			if (commands->args && !commands->next)
			{
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
			if (!commands || !commands->next && check_builtin_prepipe(commands,
					data) == true)
				continue ;
			else
				execute_pipeline(commands, data, data->envp);
		}
	}
	return (g_error);
}
