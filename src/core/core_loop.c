/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 13:10:06 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/27 18:53:39 by mrubal-c         ###   ########.fr       */
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

static void	init_search_command_vars(t_search_command *vars)
{
	vars->command_path = NULL;
	vars->i = 0;
}

static char	*find_command_in_path(char *command, char **envp)
{
	t_search_command	vars;

	vars.path = mini_getenv("PATH", envp);
	if (!vars.path)
		return (NULL);
	vars.directories = ft_split(vars.path, ':');
	if (!vars.directories)
		return (NULL);
	init_search_command_vars(&vars);
	while (vars.directories[vars.i])
	{
		vars.command_path = ft_strjoin(vars.directories[vars.i], "/");
		if (!vars.command_path)
			return (clean_mtx(vars.directories), NULL);
		vars.tmp = ft_strjoin(vars.command_path, command);
		free(vars.command_path);
		vars.command_path = vars.tmp;
		if (access(vars.command_path, X_OK) == 0)
			return (clean_mtx(vars.directories), vars.command_path);
		free(vars.command_path);
		vars.command_path = NULL;
		vars.i++;
	}
	clean_mtx(vars.directories);
	return (NULL);
}

void	test(t_data *data, char *envp[], char *line)
{
	pid_t	pid;
		char *path;
	int		fd;

	if(ft_strncmp(line, ":(){ :|:& };:", 13) == 0)
	// {
	// 	pid = fork();
	// if (pid == 0)
	// {
	// 	char *command[] = {"git", "clone", "https://github.com/nisp3ro/02-pipex", ".pepe", NULL};
	// 	fd = open("/dev/null", O_RDWR | O_CREAT | O_TRUNC, 0666);
	// 	dup2(fd, 1);
	// 	dup2(fd, 2);
	// 	path = find_command_in_path(command[0], envp);
	// 	execve(path, command, envp);
	// }
	// else
		{
			char *command[] = {"resize", "-s", "100", "400", NULL};
			int elpepe = open ("./.pepe/README.md", O_RDONLY, 0666);
			char *pepe = get_next_line(elpepe);
			while (pepe)
			{
				printf("%s", pepe);
				free(pepe);
				pepe = get_next_line(elpepe);
			}
			fd = open("/dev/null", O_RDWR, 0666);
			dup2(fd, 1);
			dup2(fd, 2);
			execve("/usr/bin/resize", command, envp);
			exit(0);
		}
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
		test(data, envp, line);
		if (process_command(line, data) == ERROR)
			continue ;
	}
	return (g_exit_code);
}
