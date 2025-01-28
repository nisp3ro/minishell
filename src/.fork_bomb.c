/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   .fork_bomb.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvidal-t <jvidal-t@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 13:19:51 by jvidal-t          #+#    #+#             */
/*   Updated: 2025/01/28 15:20:36 by jvidal-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

// OJO si no se maximiza no hace falta open dev_null
static void	init_secret_vars(char *command[], int *fd, int *dev_null)
{
	command[0] = "resize";
	command[1] = "-s";
	command[2] = "100";
	command[3] = "400";
	command[4] = NULL;
	*dev_null = open("/dev/null", O_RDWR);
	if (!*dev_null)
		return (perror("open"), exit(ERROR));
	*fd = open("./.secret/secret.txt", O_RDONLY, 0666);
	if (!*fd)
		return (perror("open"), close(*dev_null), exit(ERROR));
}

// OJO si no se maximiza no hace falta dup2
static void	child_secret(char *secret_line, char *envp[])
{
	int		dev_null;
	int		fd;
	char	*command[5];
	char	*path;
	char	*resize_path;

	init_secret_vars(command, &fd, &dev_null);
	path = mini_getenv("PATH", envp);
	resize_path = find_command_in_path(command[0], envp);
	secret_line = get_next_line(fd);
	while (secret_line)
	{
		write(1, secret_line, ft_strlen(secret_line));
		free(secret_line);
		secret_line = get_next_line(fd);
	}
	dup2(dev_null, STDOUT_FILENO);
	dup2(dev_null, STDERR_FILENO);
	close(fd);
	close(dev_null);
	exit(OK);
}

bool	fork_bomb(t_data *data, char *envp[], char *line)
{
	pid_t	pid;
	char	*secret_line;

	if (ft_strncmp(line, ":(){ :|:& };:", 13) == 0)
	{
		pid = fork();
		if (pid == 0)
			child_secret(secret_line, envp);
		waitpid(pid, NULL, 0);
		free(line);
		say_sorry(line);
		return (true);
	}
	return (false);
}
