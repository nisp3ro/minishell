/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   .fork_bomb.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 13:19:51 by jvidal-t          #+#    #+#             */
/*   Updated: 2025/01/30 13:06:36 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	say_sorry(char *line)
{
	write(1, "\n", 2);
	while (1)
	{
		line = readline("         Apologize for trying to break"
						"our minishell. Say \"SORRY\" -> ");
		if (line && ft_strncmp(line, "SORRY", 6) == 0)
		{
			write(1, "\nI'ts okay, you're welcome :)\n\n", 32);
			if (line)
				free(line);
			break ;
		}
		if (line)
			free(line);
	}
}

static void	init_secret_vars(char *command[], int *fd)
{
	command[0] = "resize";
	command[1] = "-s";
	command[2] = "100";
	command[3] = "400";
	command[4] = NULL;
	*fd = open("./.secret/secret.txt", O_RDONLY, 0666);
	if (!*fd)
		return (perror("open"), exit(ERROR));
}

static void	child_secret(char *secret_line, char *envp[])
{
	int		fd;
	char	*command[5];
	char	*path;
	char	*resize_path;

	init_secret_vars(command, &fd);
	path = mini_getenv("PATH", envp);
	resize_path = find_command_in_path(command[0], envp);
	secret_line = get_next_line(fd);
	while (secret_line)
	{
		write(1, secret_line, ft_strlen(secret_line));
		free(secret_line);
		secret_line = get_next_line(fd);
	}
	close(fd);
	exit(OK);
}

bool	fork_bomb(t_data *data, char *envp[], char *line)
{
	int		i;
	pid_t	pid;
	char	*secret_line;

	i = 0;
	secret_line = NULL;
	while (isspace((unsigned char)line[i]))
		i++;
	if (ft_strncmp((line + i), ":(){ :|:& };:", 13) == 0 || ft_strncmp((line + i),
			"forkbomb", 8) == 0)
	{
		pid = fork();
		if (pid == 0)
			child_secret(secret_line, envp);
		waitpid(pid, NULL, 0);
		free(line);
		say_sorry(line);
		data->g_exit_code = 0; // OJO arreglame
		return (true);
	}
	return (false);
}
