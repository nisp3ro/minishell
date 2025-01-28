/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_here_doc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 17:02:16 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/28 17:57:49 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	manage_here_doc(t_data *data, char **line, char *limiter, int *fd)
{
	while (line[0])
	{
		if ((ft_strncmp(line[0], line[1], (ft_strlen(line[1]) + 1)) == 0)
			|| !line[0])
		{
			if (line[0])
				free(line[0]);
			break ;
		}
		if (ft_strchr(line[0], '$') && ((limiter[1] != '\''
					&& limiter[ft_strlen(limiter) - 1] != '\'')
				&& (limiter[1] != '\"' && limiter[ft_strlen(limiter)
						- 1] != '\"')))
			line[0] = expand_variables(line[0], data->envp, data);
		write(fd[1], line[0], ft_strlen(line[0]));
		write(fd[1], "\n", 1);
		free(line[0]);
		line[0] = readline("> ");
	}
}

static void	read_n_write(t_data *data, char *limiter, int *fd)
{
	char	*line[2];

	line[0] = NULL;
	line[0] = readline("> ");
	if ((limiter[1] != '\'' && limiter[ft_strlen(limiter) - 1] != '\'')
		|| (limiter[1] != '\"' && limiter[ft_strlen(limiter) - 1] != '\"'))
	{
		if (limiter[0] == '\'')
			line[1] = ft_strtrim(limiter, "\'");
		else
			line[1] = ft_strtrim(limiter, "\"");
	}
	else
		line[1] = ft_strdup(limiter);
	manage_here_doc(data, line, limiter, fd);
	free(line[1]);
}

static void	here_doc_handler(int signal)
{
	g_exit_code = 130;
	exit(g_exit_code);
}

void	here_doc(t_data *data, char *limiter, int *fd)
{
	pid_t	reader;

	g_exit_code = 0;
	g_exit_code = getpid();
	reader = fork();
	if (reader == -1)
		exit(1);
	if (reader == 0)
	{
		wait_signal(2);
		close(fd[0]);
		read_n_write(data, limiter, fd);
		close(fd[1]);
		exit(EXIT_SUCCESS);
	}
	waitpid(reader, &g_exit_code, 0);
}
// OJO LIMPIAR A LA SALIDA¿?

void	handle_here_doc(t_command *command, t_data *data, int *here_doc_pipe)
{
	int	eof_i;

	if (pipe(here_doc_pipe) == -1)
		exit(1);
	eof_i = -1;
	while (command->eof[++eof_i])
		here_doc(data, command->eof[eof_i], here_doc_pipe);
	if (g_exit_code == SIGKILL)
	{
		g_exit_code = 130;
		return ;
	}
	close(here_doc_pipe[1]);
}
