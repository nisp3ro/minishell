#include "../include/minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// char **ft_realloc(char **envp, int size)
// {
// 	char **new;
// 	int i;

// 	i = 0;
// 	new = malloc(sizeof(char *) * (size + 1));
// 	if (new == NULL)
// 	{
// 		perror("malloc");
// 		return (NULL);
// 	}
// 	while (i < size)
// 	{
// 		new[i] = envp[i];
// 		i++;
// 	}
// 	new[i] = NULL;
// 	return (new);
// }

void	set_exp(t_data *data, char* name, char *value)
{
	int i;

	i = 0;
	while (data->envp[i])
		i++;
	data->envp = realloc(data->envp, sizeof(char *) * (i + 2));
	data->envp[i] = malloc(ft_strlen(name) + ft_strlen(value) + 2);
	if (data->envp[i] == NULL)
	{
		perror("malloc");
		return ;
	}
	ft_strcpy(data->envp[i], name);
	ft_strcat(data->envp[i], "=");
	ft_strcat(data->envp[i], value);
	data->envp[i + 1] = NULL;
}

void	ft_cd(t_command *command, t_data *data)
{
	int	i;
	int	pwd_found;
	int	oldpwd_found;

	i = 0;
	pwd_found = 0;
	oldpwd_found = 0;
	if (!data->oldpwd)
		free(data->oldpwd);
	data->oldpwd = ft_strdup(mini_getenv("PWD", data->envp));
	if (!command->args[1])
	{
		if (chdir(mini_getenv("HOME", data->envp)) == -1)
		{
			perror("cd");
			return ;
		}
	}
	if (command->args[2])
	{
		write(STDERR_FILENO, " too many arguments\n", 20);
		g_error = 1;
		return ;
	}
	else if (chdir(command->args[1]) == -1)
	{
		perror("cd");
		g_error = 1;
		return ;
	}
	data->pwd = getcwd(NULL, 0);
	if (data->pwd == NULL)
	{
		perror("getcwd");
		g_error = 1;
		return ;
	}
	while (data->envp[i] != NULL)
	{
		if (ft_strncmp(data->envp[i], "PWD=", 4) == 0)
		{
			free(data->envp[i]);
			data->envp[i] = malloc(ft_strlen("PWD=") + ft_strlen(data->pwd) + 1);
			if (data->envp[i] == NULL)
			{
				perror("malloc");
				return ;
			}
			ft_strcpy(data->envp[i], "PWD=");
			ft_strcat(data->envp[i], data->pwd);
			pwd_found = 1;
			break ;
		}
		i++;
	}
	if (!pwd_found)
	{
		data->envp[i] = malloc(ft_strlen("PWD=") + ft_strlen(data->pwd) + 1);
		if (data->envp[i] == NULL)
		{
			perror("malloc");
			return ;
		}
		ft_strcpy(data->envp[i], "PWD=");
		ft_strcat(data->envp[i], data->pwd);
		data->envp[i + 1] = NULL;
	}
	i = 0;
	while (data->envp[i] != NULL)
	{
		if (ft_strncmp(data->envp[i], "OLDPWD=", 7) == 0)
		{
			free(data->envp[i]);
			data->envp[i] = malloc(ft_strlen("OLDPWD=") + ft_strlen(data->oldpwd)
					+ 1);
			if (data->envp[i] == NULL)
			{
				perror("malloc");
				return ;
			}
			ft_strcpy(data->envp[i], "OLDPWD=");
			ft_strcat(data->envp[i], data->oldpwd);
			oldpwd_found = 1;
			break ;
		}
		i++;
	}
	if (!oldpwd_found)
	{
		data->envp[i] = malloc(ft_strlen("OLDPWD=") + ft_strlen(data->oldpwd) + 1);
		if (data->envp[i] == NULL)
		{
			perror("malloc");
			return ;
		}
		ft_strcpy(data->envp[i], "OLDPWD=");
		ft_strcat(data->envp[i], data->oldpwd);
		data->envp[i + 1] = NULL;
	}
	g_error = 0;
}

void	ft_echo(t_command *command)
{
	int	i;

	i = 0;
	if (command->args[1] != NULL && ft_strncmp("-n", command->args[1], 3) == 0)
		i++;
	i++;
	write(STDOUT_FILENO, command->args[i], ft_strlen(command->args[i]));
	while (command->args[++i])
	{
		write(STDOUT_FILENO, " ", 1);
		write(STDOUT_FILENO, command->args[i], ft_strlen(command->args[i]));
	}
	if (command->args[1] != NULL && ft_strncmp("-n", command->args[1], 3))
		write(STDOUT_FILENO, "\n", 1);
	g_error = 0;
}
void	ft_env(t_data *data)
{
	int i;
	
	i = 0;
	while (data->envp[i])
	{
		write(STDOUT_FILENO, data->envp[i], ft_strlen(data->envp[i]));
		write(STDOUT_FILENO, "\n", 1);
		i++;
	}
	g_error = 0;
}
void	unset_from_envp(t_command *command, t_data *data)
{
	int i;
	int j;

	i = 0;
	if (!command->args[1])
		return;
	while (data->envp[i] != NULL)
	{
		if (ft_strncmp(data->envp[i], command->args[1], ft_strlen(command->args[1])) == 0 && data->envp[i][ft_strlen(command->args[1])] == '=')
		{
			free(data->envp[i]);
			j = i;
			while (data->envp[j] != NULL)
			{
				data->envp[j] = data->envp[j + 1];
				j++;
			}
			break;
		}
		i++;
	}
	g_error = 0;
}

void	unset_from_vars(t_command *command, t_vars **vars)
{
	t_vars *tmp;
	t_vars *prev;

	tmp = *vars;
	prev = NULL;
	while (tmp)
	{
		if (ft_strncmp(tmp->name, command->args[1], ft_strlen(command->args[1])) == 0 && tmp->name[ft_strlen(command->args[1])] == '\0')
		{
			if (prev)
				prev->next = tmp->next;
			else
				*vars = tmp->next;
			free(tmp->name);
			free(tmp->value);
			free(tmp);
			return ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}

void ft_unset(t_command *command, t_data *data)
{
    int i;
    int j;

	i = 0;
	if (!command->args[1])
		return;
	unset_from_envp(command, data);
	unset_from_vars(command, &data->vars);
	unset_from_vars(command, &data->exp_vars);
	g_error = 0;
}
void	ft_pwd(t_data *data)
{
	write(STDOUT_FILENO, data->pwd, ft_strlen(data->pwd));
	write(STDOUT_FILENO, "\n", 1);
	g_error = 0;
}
void	ft_exit(t_data *data, t_command *command)
{
	int i;
	int j;
	int num;

	i = 0;
	j = 0;
	write(STDOUT_FILENO, "exit\n", 5);
	if (command->args[1] && command->args[2])
	{
		write(STDERR_FILENO, " too many arguments\n", 20);
		exit(1);
	}
	if (command->args[1])
	{
		while (command->args[1][i])
		{
			if (!ft_isdigit(command->args[1][i]) && command->args[1][i] != '-' && command->args[1][i] != '+')
			{
				write(STDERR_FILENO, " numeric argument required\n", 27);
				exit(2);
			}
			i++;
		}
		num = ft_atoi(command->args[1]);
		while (num < 0)
			num += 256;
		g_error = num; 
	}
	else
		g_error = 0;
	exit(g_error);
}

int is_valid_identifier(const char *str)
{
	int i;

	i = 0;
    if (ft_isdigit(str[0]) || str[0] == '=')
        return (ERROR);
    while (str[i] != '\0' && str[i] != '=') 
	{
        if (!ft_isalnum(str[i]) && str[i] != '_')
            return (ERROR);
        i++;
    }

    return (OK);
}

void	ft_export(t_command *command, t_data *data)
{
	t_vars *tmp;
	char *to_export;
	int i;
	
	i = 1;

	while (command->args[i])
	{
		if (is_valid_identifier(command->args[i]) == ERROR)
		{
			write(STDERR_FILENO, " not a valid identifier\n", 24);
			g_error = 1;
			i++;
			continue ;
		}
		to_export = mini_getenv(command->args[i], data->envp);
		if (to_export)
			set_variable(&data->exp_vars, command->args[i], to_export);
		else if (!mini_getvars(data->vars, command->args[i]))
		{
			set_variable(&data->exp_vars, command->args[i], "");
			set_exp(data, command->args[i], "");
		}
		else if (mini_getvars(data->vars, command->args[i]))
		{
			to_export = mini_getvars(data->vars, command->args[i]);
			set_exp(data, command->args[i], to_export);
			set_variable(&data->exp_vars, command->args[i], mini_getenv(command->args[i], data->envp));
			unset_from_vars(command, &data->vars);
		}
		i++;
		g_error = 0;
	}
	if (i == 1)
	{
		tmp = data->exp_vars;
		while (tmp != NULL)
		{
			write(STDOUT_FILENO, "declare -x ", 11);
			write(STDOUT_FILENO, tmp->name, ft_strlen(tmp->name));
			write(STDOUT_FILENO, "=\"", 2);
			write(STDOUT_FILENO, tmp->value, ft_strlen(tmp->value));
			write(STDOUT_FILENO, "\"\n", 2);
			tmp = tmp->next;
		}
		g_error = 0;
	}
}

bool	check_builtin(t_command *command, t_data *data)
{
	if (!command->args)
		return (false);
	if (ft_strncmp(command->args[0], "exit", 5) == 0)
		return (ft_exit(data, command), true);
	if (ft_strncmp(command->args[0], "echo", 5) == 0)
		return (ft_echo(command), true);
	if (ft_strncmp(command->args[0], "cd", 3) == 0)
		return (ft_cd(command, data), true);
	if (ft_strncmp(command->args[0], "pwd", 4) == 0)
		return(ft_pwd(data), true);
	if (ft_strncmp(command->args[0], "export", 7) == 0)
		return(ft_export(command, data), true);
	if (ft_strncmp(command->args[0], "unset", 6) == 0)
		return (ft_unset(command, data), true);
	if (ft_strncmp(command->args[0], "env", 4) == 0)
		return (ft_env(data), true);
	return (false);
}

bool	check_builtin_prepipe(t_command *command, t_data *data)
{
	if (!command->args)
		return (false);
	if (ft_strncmp(command->args[0], "exit", 5) == 0)
		return (ft_exit(data, command), true);
	if (ft_strncmp(command->args[0], "cd", 3) == 0)
		return (ft_cd(command, data), true);
	if (ft_strncmp(command->args[0], "export", 7) == 0)
		return(ft_export(command, data), true);
	if (ft_strncmp(command->args[0], "unset", 6) == 0)
		return (ft_unset(command, data), true);
	return (false);
}


