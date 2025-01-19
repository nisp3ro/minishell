#include "../include/minishell.h"

int		g_error;

void	set_last_cmd_arg(t_data *data, char *name, char *value)
{
	int	i;

	i = 0;
	while (data->envp[i] != NULL)
	{
		if (ft_strncmp(data->envp[i], name, ft_strlen(name)) == 0
			&& data->envp[i][ft_strlen(name)] == '=')
		{
			free(data->envp[i]);
			data->envp[i] = malloc(ft_strlen(name) + ft_strlen(value) + 2);
			if (data->envp[i] == NULL)
			{
				perror("malloc");
				return ;
			}
			ft_strcpy(data->envp[i], name);
			ft_strcat(data->envp[i], "=");
			ft_strcat(data->envp[i], value);
			return ;
		}
		i++;
	}
	if (set_exp(data, name, value) == ERROR)
		return ; //limpiar y ver y tal
}

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		if (tmp->type == TOKEN_WORD)
			free(tmp->value);
		free(tmp);
	}
}
char	*ft_get_user_home(t_data *data)
{
	DIR				*dir;
	struct dirent	*entry;
	char			*home_path;
	char			*username;
	
	username = mini_getenv("HOME", data->envp);
	if (username != NULL)
	{
		username = ft_strjoin(username, "/.minishell_history");
		return (username);
	}
	dir = opendir("/home");
	if (dir == NULL)
		return (NULL);
	while ((entry = readdir(dir)) != NULL)
	{
		if (ft_strncmp(entry->d_name, "Desktop", 8) != 0)
		{
			home_path = malloc(strlen("/home/") + strlen(entry->d_name) + 20 + 1);
			if (home_path == NULL)
				return (closedir(dir), NULL);
			strcpy(home_path, "/home/");
			strcat(home_path, entry->d_name);
			strcat(home_path, "/.minishell_history");
			if (access(home_path, F_OK) == 0)
			{
				closedir(dir);
				return (home_path);
			}
		}
	}
	closedir(dir);
	return (NULL);
}

void	ft_save_history(t_data *data)
{
	int		i;
	char	*usr_path;

	usr_path = ft_get_user_home(data);
	if (usr_path == NULL)
		return ;
	data->fd = open(usr_path, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (data->fd == -1)
		return ;
	i = 0;
	while (i < data->hist_size)
	{
		write(data->fd, data->history[i], ft_strlen(data->history[i]));
		write(data->fd, "\n", 1);
		i++;
	}
	close(data->fd);
}

void	ft_write_history(t_data *data, char *line)
{
	int	i;

	i = 0;
	add_history(line);
	if (data->hist_size >= 1000)
	{
		while (i < 1000)
		{
			ft_strcpy(data->history[i], data->history[i + 1]);
			i++;
		}
		ft_strcpy(data->history[999], line);
		ft_save_history(data);
		return ;
	}
	ft_strcpy(data->history[data->hist_size], line);
	data->hist_size++;
	ft_save_history(data);
}

int	interactive_mode(t_data *data, char *envp[])
{
	char		*line;
	char		*full_cmd;
	t_token		*tokens;
	t_command	*commands;
	char		*tmp;
	char		*tmp2;
	int			i;

	i = 0;
	while (1)
	{
		//if (isatty(STDIN_FILENO))
		if (get_prompt(&data->prompt, data) == ERROR)
			return (ERROR);
		line = readline(data->prompt);
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		// else
		// {
		// 	line = readline("");
		// 	write(STDOUT_FILENO, "\033[A\r", 4);
		// 	if (!line)
		// 		break ;
		// }
		while (isspace((unsigned char)line[i]))
			i++;
		if (line[i] == '|')
		{
			printf("minishell: syntax error near unexpected token `|'\n");
			g_error = 258;
			free(line);
			continue ;
		}
		while (1)
		{
			i = ft_strlen(line) - 1;
			while (isspace((unsigned char)line[i]))
				i--;
			if (line[i] == '|')
			{
				tmp = readline(">");
				tmp2 = ft_strjoin(line, " ");
				free(line);
				line = ft_strjoin(tmp2, tmp);
				free(tmp);
				free(tmp2);
			}
			else
				break ;
		}
		if (line[0] != '\0' && !is_all_spaces(line))
			ft_write_history(data, line);
		full_cmd = ft_strtrim(line, " ");
		free(line);
		if (full_cmd == NULL)
			return (perror("Error"), ERROR);
		else if (full_cmd[0] != '\0')
		{
			tokens = tokenize(full_cmd, data);
			free(full_cmd);
			if (tokens == NULL)
				continue ;
			commands = parse_pipeline(data, tokens);
			free_tokens(tokens);
			if (commands == NULL)
				continue ;
			if (commands->args && !commands->next)
			{
				if (ft_strncmp(commands->args[0], "export", 7) == 0
					&& commands->args[1] == NULL)
					set_last_cmd_arg(data, "_", commands->args[0]);
				else if (ft_strncmp(commands->args[0], "export", 7) != 0)
				{
					i = 0;
					while (commands->args[i])
						i++;
					set_last_cmd_arg(data, "_", commands->args[i - 1]);
				}
			}
			if (!commands || !commands->next && check_builtin_prepipe(commands,
					data) == true)
				continue ;
			else
				execute_pipeline(commands, data, data->envp);
		}
	}
	return (g_error);
}

void	ft_init_history(char history[1001][1024])
{
	int	i;
	int	j;

	i = 0;
	while (i < 1001)
	{
		j = 0;
		while (j < 1024)
		{
			history[i][j] = '\0';
			j++;
		}
		i++;
	}
}

void	ft_recover_history(t_data *data)
{
	int		i;
	char	*line;
	int		x;
	char	*usr_path;

	ft_init_history(data->history);
	data->hist_size = 0;
	line = NULL;
	usr_path = ft_get_user_home(data);
	if (usr_path == NULL)
		return ;
	data->fd = open(usr_path, O_RDONLY);
	if (data->fd == -1)
		return ;
	line = get_next_line(data->fd);
	while (line != NULL)
	{
		ft_strcpy(data->history[data->hist_size], line);
		i = 0;
		while (data->history[data->hist_size][i] != '\0')
		{
			if (data->history[data->hist_size][i] == '\n')
			{
				data->history[data->hist_size][i] = '\0';
				break ;
			}
			i++;
		}
		add_history(data->history[data->hist_size]);
		free(line);
		line = get_next_line(data->fd);
		data->hist_size++;
	}
	close(data->fd);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_data data;

	g_error = 0;
	if (argc == 2 || (argc > 2 && ft_strncmp(argv[1], "-c", 3) != 0))
		return (write(STDERR_FILENO, "Error: Argument is not -c.\n", 28), 127);
	if (init_data(&data, envp) == ERROR)
		return (perror("Error"), 1);
	ft_recover_history(&data);
	wait_signal(1);
	if (argc == 1)
	{
		if (interactive_mode(&data, data.envp) == ERROR)
			return (ERROR); // return(limpiar, 1)
	}
	else
	{
		t_token *tokens = tokenize(argv[2], &data);
		t_command *commands = parse_pipeline(&data, tokens);
		if (!commands->next && check_builtin(commands, &data) == true)
			return (OK); // return(limpiar, 0)
		execute_pipeline(commands, &data, data.envp);
	}
	rl_clear_history();
	return (g_error); // return(limpiar, 0)
}