#include "../include/minishell.h"

// Función para liberar el array de cadenas (split)
void	ft_free_split(char **split)
{
	int	i;

	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

char	*find_command_in_path(char *command, char **envp)
{
	char	*path;
	char	**directories;
	char	*command_path;
	int		i;
	char	*tmp;

	path = mini_getenv("PATH", envp);
	if (!path)
		return (NULL);
	// Dividir el PATH en directorios
	directories = ft_split(path, ':');
	if (!directories)
		return (NULL);
	command_path = NULL;
	i = 0;
	// Buscar el comando en cada directorio
	while (directories[i])
	{
		// Construir la ruta completa
		command_path = ft_strjoin(directories[i], "/");
		if (!command_path)
			return (ft_free_split(directories), NULL);
		tmp = ft_strjoin(command_path, command);
		free(command_path);
		command_path = tmp;
		// Verificar si el archivo existe y tiene permisos de ejecución
		if (access(command_path, X_OK) == 0)
			return (ft_free_split(directories), command_path);
		// Si se encontró, retornar la ruta
		free(command_path);
		command_path = NULL;
		i++;
	}
	ft_free_split(directories);
	return (NULL); // Si no se encontró el comando
}

void ft_create_custom_path(char **path, t_command *command)
{
    int i;
    int len;

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

	//clear_lst(command);
	while (i)
	{
		temp_pid = waitpid(-1, &g_error, 0);
		if (WIFEXITED(g_error))
			g_error = WEXITSTATUS(g_error);
		if (temp_pid == pid)
			temp = g_error;
		if (g_error == 2 || g_error == 3)
			g_error = g_error + 128;
		else if (g_error != 0 && g_error != 1 && g_error != 127
			&& g_error != 13 && g_error != 126)
			perror(NULL);
		i--;
	}
	g_error = temp;
	wait_signal(1);
}

void	read_n_write(t_data *data, char *limiter, int *fd)
{
	char	*line;
	char	*tmp;

	line = readline("heredoc> ");
	if ((limiter[1] != '\'' && limiter[ft_strlen(limiter) - 1] != '\'') || (limiter[1] != '\"' && limiter[ft_strlen(limiter) - 1] != '\"'))
	{
		if (limiter[0] == '\'')
			tmp = ft_strtrim(limiter, "\'");
		else
			tmp = ft_strtrim(limiter, "\"");
	}
	else
		tmp = ft_strdup(limiter);
	while (line)
	{
		if ((ft_strncmp(line, tmp, (ft_strlen(tmp) + 1)) == 0) || !line)
		{
			if(line)
				free(line);
			break ;
		}
		if (ft_strchr(line, '$') && ((limiter[1] != '\'' && limiter[ft_strlen(limiter) - 1] != '\'') && (limiter[1] != '\"' && limiter[ft_strlen(limiter) - 1] != '\"')))
			line = expand_variables(line, data->envp, data);
		write(fd[1], line, ft_strlen(line));
		write(fd[1], "\n", 1);
		free(line);
		line = readline("heredoc> ");
	}
	free(tmp);
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

// Función para ejecutar una tubería de comandos con fork usando execve
void	execute_pipeline(t_command *command, t_data *data, char **envp)
{
	int 	i;
	int		pipefd[2];
	pid_t	pid;
	int		in_fd;
	char	*command_path;
	int		fd_in;

	in_fd = STDIN_FILENO;
	wait_signal(0);
	i = 0;
	while (command)
	{
		i++;
		// Crear el pipe solo si hay más de un comando
		if (command->next)
		{
			if (pipe(pipefd) == -1)
			{
				perror("pipe");
				exit(EXIT_FAILURE); // limpiar
			}
		}
		if ((pid = fork()) == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE); // limpiar
		}
		if (pid == 0)
		{ // Proceso hijo
			if (command->args && ft_strchr(command->args[0], '/') != 0)
			{
				ft_create_custom_path(&command_path, command);
			}
			else if (command->args)
				command_path = find_command_in_path(command->args[0], envp);
			// Redirigir la entrada si no es el primer comando
			if (in_fd != STDIN_FILENO)
			{
				dup2(in_fd, STDIN_FILENO); // Redirigir stdin
				close(in_fd);              // Cerrar el descriptor de entrada
			}
			// Redirigir la salida si es un comando intermedio o final

			// Redirección de entrada
			if (command->eof != NULL)
				here_doc(data, command->eof);
			else if (command->input_redirection)
			{
					fd_in = command->input_redirection;
					dup2(fd_in, STDIN_FILENO);
					close(fd_in);
			}
			if (command->next)
			{
				dup2(pipefd[1], STDOUT_FILENO); // Redirigir stdout al pipe
				close(pipefd[0]);
				// Cerrar el extremo de lectura del pipe
			}
			// Redirección de salida
			if (command->output_redirection)
			{
				dup2(command->output_redirection, STDOUT_FILENO);
				close(command->output_redirection);
			}
			if (command->args && check_builtin(command, data) == false)
			{ // Ejecutar el comando con execve
				execve(command_path, command->args, envp);
				write(STDERR_FILENO, " command not found\n", 19);
				g_error = 127;
				exit(g_error);
			}
			exit(g_error);
		}
		else
		{ // Proceso padre
			// Cerrar los descriptores de pipes después de ser usados
			if (in_fd != STDIN_FILENO)
				close(in_fd);
			if (command->next)
			{
				close(pipefd[1]); // Cerrar el descriptor de escritura del pipe
				in_fd = pipefd[0];
				// Establecer la entrada para el siguiente comando
			}
		}
		command = command->next; // Pasar al siguiente comando
	}
	wait_exit(i, pid, &command);
}