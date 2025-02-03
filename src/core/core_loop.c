/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 13:10:06 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/02/03 10:07:28 by mrubal-c         ###   ########.fr       */
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
	if (check_cmd_start(data, input, i) == ERROR)
		return (ERROR);
	line[0] = unfinished_pipe(input);
	if (line[0] == NULL)
		return (ERROR);
	line[1] = clean_line(line[0], data);
	if (line[1] == NULL)
		return (ERROR);
	line[0] = expand_variables_pretoken(line[1], data);
	free(line[1]);
	if (line[0] == NULL)
		return (ERROR);
	if (line[0][0] != '\0')
		token_parsec_exec(line[0], data, true);
	else
		free(line[0]);
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
			return (printf("exit\n"), data->g_exit_code);
		if (fork_bomb(data, envp, line))
			continue ;
		if (process_command(line, data) == ERROR)
			continue ;
	}
	return (data->g_exit_code);
}
