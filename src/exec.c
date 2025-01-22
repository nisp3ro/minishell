/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvidal-t <jvidal-t@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 13:40:12 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/22 19:30:23 by jvidal-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	init_search_command_vars(t_search_command *vars)
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

static void	ft_create_custom_path(char **path, t_command *command)
{
	int	i;
	int	len;

	*path = command->args[0];
	len = ft_strlen(command->args[0]);
	i = len - 1;
	while (i >= 0 && command->args[0][i] != '/')
		i--;
	command->args[0] += i;
}

static void	wait_exit(int i, int pid, t_command **command)
{
	int	temp_pid;
	int	temp;

	// clear_lst(command);
	while (i)
	{
		temp_pid = waitpid(-1, &g_exit_code, 0);
		if (WIFEXITED(g_exit_code))
			g_exit_code = WEXITSTATUS(g_exit_code);
		if (temp_pid == pid)
			temp = g_exit_code;
		if (g_exit_code == 2 || g_exit_code == 3)
			g_exit_code = g_exit_code + 128;
		else if (g_exit_code != 0 && g_exit_code != 1 && g_exit_code != 127
			&& g_exit_code != 13 && g_exit_code != 126)
			perror(NULL);
		i--;
	}
	g_exit_code = temp;
	wait_signal(1);
}

void	manage_here_doc(t_data *data, char **line, char *limiter, int *fd)
{
	while (line[0])
	{
		if ((ft_strncmp(line[0], line[1], (ft_strlen(line[1]) + 1)) == 0) || !line[0])
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
		line[0] = readline("heredoc> ");
	}
}

static void	read_n_write(t_data *data, char *limiter, int *fd)
{
	char	*line[2];

	line[0] = readline("heredoc> ");
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

void	here_doc(t_data *data, char *limiter)
{
	pid_t	reader;
	int		fd[2];

	if (pipe(fd) == -1)
		exit(1); // limpiar
	reader = fork();
	if (reader == -1)
		exit(1); // limpiar
	if (reader == 0)
	{
		close(fd[0]);
		read_n_write(data, limiter, fd);
		close(fd[1]);
		exit(EXIT_SUCCESS);
	}
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	waitpid(reader, NULL, 0);
}

void	redir(t_command *command, t_redir_vars *red)
{
	if (command->redir->type == INPUT)
	{
		red->input_redirection = true;
		if (red->fd_in != STDIN_FILENO)
			close(red->fd_in);
		red->fd_in = open(command->redir->value, O_RDONLY);
		if (red->fd_in < 0)
			return (perror("open"), exit(EXIT_FAILURE)); // limpiar
	}
	else if (command->redir->type == OUTPUT)
	{
		red->output_redirection = true;
		if (red->fd_out != STDOUT_FILENO)
			close(red->fd_out);
		if (command->append)
			red->fd_out = open(command->redir->value,
					O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			red->fd_out = open(command->redir->value,
					O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (red->fd_out < 0)
			return (perror("open"), exit(EXIT_FAILURE)); // limpiar
	}
	command->redir = command->redir->next;
}

void	init_redir_vars(t_redir_vars *red)
{
	red->fd_in = STDIN_FILENO;
	red->fd_out = STDOUT_FILENO;
	red->input_redirection = false;
	red->output_redirection = false;
}

void	manage_redirections(t_redir_vars *red)
{
	if (red->input_redirection)
	{
		dup2(red->fd_in, STDIN_FILENO);
		close(red->fd_in);
	}
	if (red->output_redirection)
	{
		dup2(red->fd_out, STDOUT_FILENO);
		close(red->fd_out);
	}
}

char	*manage_redirs(t_command *command, char **envp, int in_fd,
		int pipefd[2], t_data *data)
{
	t_redir_vars	red;

	init_redir_vars(&red);
	if (command->args && ft_strchr(command->args[0], '/') != 0)
		ft_create_custom_path(&red.command_path, command);
	else if (command->args)
		red.command_path = find_command_in_path(command->args[0], envp);
	if (in_fd != STDIN_FILENO)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	if (command->eof != NULL)
		here_doc(data, command->eof);
	while (command->redir)
		redir(command, &red); // command redir->next?
	if (command->next)
	{
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[0]);
	}
	manage_redirections(&red);
	return (red.command_path);
}

void	execve_error_exit(t_command *command, char *command_path)
{
	if (errno == EACCES)
	{
		if (access(command_path, X_OK) != 0)
			write(STDERR_FILENO, " Permission denied\n", 19);
		else
			write(STDERR_FILENO, " Is a directory\n", 16);
		exit(126);
	}
	else if (errno == ENOEXEC)
		return (write(STDERR_FILENO,
				" Exec format error. Wrong Architecture.\n", 40), exit(126));
	else if (errno == EISDIR)
		return (write(STDERR_FILENO, " Is a directory\n", 16), exit(126));
	else if (errno == ENOTDIR)
		return (write(STDERR_FILENO, " Not a directory\n", 17), exit(126));
	else if (errno == ENOENT)
		return (write(STDERR_FILENO, " No such file or directory\n", 27),
			exit(127));
	else
		return (write(STDERR_FILENO, command->args[0],
				ft_strlen(command->args[0])), write(STDERR_FILENO,
				": command not found\n", 20), exit(127));
}

void	init_pip(t_pip_vars *pip)
{
	pip->i = 0;
	pip->in_fd = STDIN_FILENO;
}

void	father_process(t_pip_vars *pip, t_command *command)
{
	if (pip->in_fd != STDIN_FILENO)
		close(pip->in_fd);
	if (command->next)
	{
		close(pip->pipefd[1]);
		pip->in_fd = pip->pipefd[0];
	}
}

void	execute_pipeline(t_command *command, t_data *data, char **envp)
{
	t_pip_vars	pip;

	init_pip(&pip);
	while (command != NULL)
	{
		pip.i++;
		if (command->next)
			if (pipe(pip.pipefd) == -1)
				return (perror("pipe"), exit(EXIT_FAILURE)); // limpiar
		if ((pip.pid = fork()) == -1)
			return (perror("fork"), exit(EXIT_FAILURE)); // limpiar
		if (pip.pid == 0)
		{
			pip.command_path = manage_redirs(command, envp, pip.in_fd,
					pip.pipefd, data);
			if ((command->args && check_builtin(command, data) == false)
				&& execve(pip.command_path, command->args, envp))
				execve_error_exit(command, pip.command_path);
			exit(g_exit_code);
		}
		else
			father_process(&pip, command);
		command = command->next;
	}
	wait_exit(pip.i, pip.pid, &command);
}
