/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvidal-t <jvidal-t@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 13:10:06 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/28 14:36:04 by jvidal-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	process_command(char *input, t_data *data)
{
	char	*line[2];
	int		i;

	i = 0;
	while (isspace((unsigned char)input[i]))
		i++;
	if (check_cmd_start(input, i) == ERROR)
		return (ERROR);
	line[0] = unfinished_pipe(input);
	if (line[0] == NULL)
		return (ERROR);
	line[1] = clean_line(line[0], data);
	if (line[1] == NULL)
		return (ERROR);
	if (line[1][0] != '\0')
		token_parsec_exec(line[1], data);
	else
		free(line[1]);
	return (OK);
}

int	interactive_mode(t_data *data, char *envp[])
{
	char	*line;

	while (1)
	{
		if (get_prompt(&data->prompt, data) == ERROR)
			return (ERROR);
		line = readline(data->prompt);
		free(data->prompt);
		data->prompt = NULL;
		if (!line)
			return (printf("exit\n"), g_exit_code);
		if (fork_bomb(data, envp, line))
			continue ;
		if (process_command(line, data) == ERROR)
			continue ;
	}
	return (g_exit_code);
}
