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
} // .h

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
} // .h

void	fork_bomb(t_data *data, char *envp[], char *line)
{
	pid_t	pid;
	char	*command[] = {"resize", "-s", "100", "400", NULL}; //init norminette
	int		fd;
	char	*secret_line;

	if (ft_strncmp(line, ":(){ :|:& };:", 13) == 0)
	{
		pid = fork();
		if(pid == 0)
		{
			fd = open("./.secret/secret.txt", O_RDONLY, 0666);
			secret_line = get_next_line(fd);
			while (secret_line)
			{
				printf("%s", secret_line);
				free(secret_line);
				secret_line = get_next_line(fd);
			}
			close(fd);
			// execve("/usr/bin/resize", command, envp); //getenv path?
			// perror("execve");
		}
		waitpid(pid, NULL, 0);
		exit(0);//needed?
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
		fork_bomb(data, envp, line); //limpiar
		if (process_command(line, data) == ERROR)
			continue ;
	}
	return (g_exit_code);
}
