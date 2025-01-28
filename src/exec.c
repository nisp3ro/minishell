/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvidal-t <jvidal-t@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 13:40:12 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/28 13:17:49 by jvidal-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	init_search_command_vars(t_search_command *vars)
{
	vars->command_path = NULL;
	vars->i = 0;
}

char	*find_command_in_path(char *command, char **envp)
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

	clean_cmd(*command);
	while (i >= 0)
	{
		temp_pid = waitpid(-1, &g_exit_code, 0);
		if (WIFEXITED(g_exit_code))
			g_exit_code = WEXITSTATUS(g_exit_code);
		if (temp_pid == pid)
			temp = g_exit_code;
		if (g_exit_code == 2 || g_exit_code == 3)
			g_exit_code = g_exit_code + 128;
		i--;
	}
	g_exit_code = temp;
	wait_signal(1);
}

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
		exit(1); // limpiar
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
								O_WRONLY | O_CREAT | O_APPEND,
								0644);
		else
			red->fd_out = open(command->redir->value,
								O_WRONLY | O_CREAT | O_TRUNC,
								0644);
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

char	*manage_redirs(t_command *command, char **envp, t_pip_vars *pip,
		t_data *data)
{
	t_redir_vars	red;
	int				eof_i;
	int				here_doc_pipe[2];

	init_redir_vars(&red);
	if (command->args && ft_strchr(command->args[0], '/') != 0)
		ft_create_custom_path(&red.command_path, command);
	else if (command->args)
		red.command_path = find_command_in_path(command->args[0], envp);
	if (pip->in_fd != STDIN_FILENO)
	{
		dup2(pip->in_fd, STDIN_FILENO);
		close(pip->in_fd);
	}
	while (command->redir)
		redir(command, &red);
	if (command->next)
	{
		dup2(pip->pipefd[1], STDOUT_FILENO);
		close(pip->pipefd[0]);
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
						" Exec format error. Wrong Architecture.\n",
						40),
				exit(126));
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

void	init_pip(t_pip_vars *pip, t_command **command)
{
	pip->i = 0;
	pip->in_fd = STDIN_FILENO;
	pip->command_head = *command;
	wait_signal(0);
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

bool	check_cmd_args(t_command *command)
{
	if (command->args && command->args[0] != NULL
		&& command->args[0][0] != '\0')
		return (true);
	return (false);
}

void	execute_pipeline(t_command *command, t_data *data, char **envp)
{
	t_pip_vars	pip;
	int			eof_i;
	int			here_doc_pipe[2];

	init_pip(&pip, &command);
	while (command != NULL)
	{
		pip.i++;
		if (command->next)
			if (pipe(pip.pipefd) == -1)
				return (perror("pipe"), exit(EXIT_FAILURE)); // limpiar
		if (command->eof != NULL)
		{
			if (pipe(here_doc_pipe) == -1)
				exit(1); // limpiar
			eof_i = -1;
			while (command->eof[++eof_i])
				here_doc(data, command->eof[eof_i], here_doc_pipe);
			if (g_exit_code == SIGKILL)
			{
				g_exit_code = 130;
				return ;
			}
			close(here_doc_pipe[1]);
			pip.in_fd = here_doc_pipe[0];
		}
		g_exit_code = 0;
		if ((pip.pid = fork()) == -1)
			return (perror("fork"), exit(EXIT_FAILURE)); // limpiar
		if (pip.pid == 0)
		{
			pip.command_path = manage_redirs(command, envp, &pip, data);
			if ((check_cmd_args(command) && !check_builtin(command, data))
				&& execve(pip.command_path, command->args, envp))
				execve_error_exit(command, pip.command_path);
			exit(g_exit_code);
		}
		else
		{
			if (command->eof != NULL)
				waitpid(pip.pid, &g_exit_code, 0);
			father_process(&pip, command);
		}
		command = command->next;
	}	
	wait_exit(pip.i, pip.pid, &pip.command_head);
}
