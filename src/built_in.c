#include "../include/minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

bool	mark_as_exported(t_vars *vars, const char *name)
{
	t_vars *current = vars;
	while (current != NULL)
	{
		if (current->name && strcmp(current->name, name) == 0)
		{
			current->exported = true;
			return (true);
		}
		current = current->next;
	}
	return (false);
}

void	ft_export(t_command *command, t_data *data)
{
	t_vars *tmp;
	char *to_export;
	int i;
	
	i = 1;
	while (command->args[i])
	{
		to_export = mini_getenv(command->args[1], data->envp);
		if (to_export)
			set_variable(&data->vars, command->args[i], to_export);
		else if (!mini_getvars(data->vars, command->args[i]))
			set_variable(&data->vars, command->args[i], "");
		mark_as_exported(data->vars, command->args[i]);
		i++;
	}
	if (i == 1)
	{
		tmp = data->vars;
		while (tmp)
		{
			if(tmp->exported)
			{
				printf("declare -x %s", tmp->name);
				if (tmp->value[0] != '\0')
					printf("=\"%s\"", tmp->value);
				printf("\n");
			}
			tmp = tmp->next;
		}
	}
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
	else if (chdir(command->args[1]) == -1)
	{
		perror("cd");
		return ;
	}
	data->pwd = getcwd(NULL, 0);
	if (data->pwd == NULL)
	{
		perror("getcwd");
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
}

void	ft_echo(t_command *command)
{
	int	i;

	i = 0;
	if (command->args[1] != NULL && ft_strcmp("-n", command->args[1]) == 0)
		i++;
	while (command->args[++i])
	{
		write(STDOUT_FILENO, command->args[i], ft_strlen(command->args[i]));
		if (command->args[i + 1])
			write(STDOUT_FILENO, " ", 1);
	}
	if (command->args[1] != NULL && ft_strcmp("-n", command->args[1]))
		write(STDOUT_FILENO, "\n", 1);
	if (command->isfather == false)
		exit(OK);
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
}
void ft_unset(t_command *command, t_data *data)
{
    int i;
    int j;

	i = 0;
	if (!command->args[1])
		return;
    while (data->envp[i] != NULL)
    {
        if (ft_strncmp(data->envp[i], command->args[1], ft_strlen(command->args[1])) == 0)
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
}


bool	check_builtin(t_command *command, t_data *data)
{
	if (!command->args)
		return (false);
	if (strcmp(command->args[0], "echo") == 0)
		return (ft_echo(command), true);
	if (strcmp(command->args[0], "cd") == 0)
		return (ft_cd(command, data), true);
	// if (strcmp(command->args[0], "pwd") == 0)
	if (strcmp(command->args[0], "export") == 0)
		return(ft_export(command, data), true);
	if (strcmp(command->args[0], "unset") == 0)
		return (ft_unset(command, data), true);
	if (strcmp(command->args[0], "env") == 0)
		return (ft_env(data), true);
	return (false);
}
