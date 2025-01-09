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
			return (ft_free_split(directories), command_path); // Si se encontró, retornar la ruta
		free(command_path);
		command_path = NULL;
		i++;
	}
	ft_free_split(directories);
	return (NULL); // Si no se encontró el comando
}

// Función para ejecutar una tubería de comandos con fork usando execve
void	execute_pipeline(t_command *command, char **envp)
{
	int		pipefd[2];
	pid_t	pid;
	int		in_fd;
	char	*command_path;
	int		fd_in;
	int		fd_out;

	in_fd = STDIN_FILENO;
	while (command)
	{
		// Crear el pipe solo si hay más de un comando
		if (command->next)
		{
			if (pipe(pipefd) == -1)
			{
				perror("pipe");
				exit(EXIT_FAILURE); //limpiar
			}
		}
		if ((pid = fork()) == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE); //limpiar
		}
		if (pid == 0)
		{ // Proceso hijo
			command_path = find_command_in_path(command->args[0], envp);
			// Redirigir la entrada si no es el primer comando
			if (in_fd != STDIN_FILENO)
			{
				dup2(in_fd, STDIN_FILENO); // Redirigir stdin
				close(in_fd);              // Cerrar el descriptor de entrada
			}
			// Redirigir la salida si es un comando intermedio o final
			if (command->next)
			{
				dup2(pipefd[1], STDOUT_FILENO); // Redirigir stdout al pipe
				close(pipefd[0]);              
				// Cerrar el extremo de lectura del pipe
			}
			// Redirección de entrada
			if (command->input_redirection)
			{
				fd_in = open(command->input_redirection, O_RDONLY);
				if (fd_in < 0)
				{
					perror("open input");
					exit(EXIT_FAILURE);
				}
				dup2(fd_in, STDIN_FILENO);
				close(fd_in);
			}
			// Redirección de salida
			if (command->output_redirection)
			{
                if (command->append)
				    fd_out = open(command->output_redirection, O_WRONLY | O_CREAT | O_APPEND, 0644);
				else
                    fd_out = open(command->output_redirection, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd_out < 0)
				{
					perror("open output");
					exit(EXIT_FAILURE);
				}
				dup2(fd_out, STDOUT_FILENO);
				close(fd_out);
			}
			// Ejecutar el comando con execve
			execve(command_path, command->args, envp);
            perror("execve");
            exit(EXIT_FAILURE);
		}
		else
		{ // Proceso padre
			// Esperar a que termine el hijo y cerrar los pipes
			waitpid(pid, NULL, 0);
			// Cerrar los descriptores de pipes después de ser usados
			if (in_fd != STDIN_FILENO)
				close(in_fd);
			if (command->next)
			{
				close(pipefd[1]);  // Cerrar el descriptor de escritura del pipe
				in_fd = pipefd[0];
					// Establecer la entrada para el siguiente comando
			}
		}
		command = command->next; // Pasar al siguiente comando
	}
}
