#include "../include/minishell.h"

/**
 * host_trim - Trims a hostname to exclude domain and directory information.
 * 
 * This function processes a given hostname string to remove the domain part 
 * (after the first dot) and any directory path (before the first slash). It 
 * returns the trimmed portion of the hostname, which represents only the base 
 * hostname without additional details.
 * 
 * Parameters:
 *    - host: A pointer to the original hostname string.
 * 
 * Return:
 *    - A dynamically allocated string containing the trimmed hostname, 
 *      or NULL if memory allocation fails.
 * 
 * Note:
 *    - The caller is responsible for freeing the allocated memory for the 
 *      returned string.
 */
char	*host_trim(char *host)
{
	char	*trimmed;
	int		i;
	int		j;
	int		k;

	i = 0;
	j = 0;
	while (host[i] != '\0' && host[i++] != '/')
		j = i;
	while (host[++j] != '\0' && host[j] != '.')
		trimmed = malloc(sizeof(char) * (j - i + 1));
	if (!trimmed)
		return (NULL);
	k = 0;
	while (i < j)
		trimmed[k++] = host[i++];
	trimmed[k] = '\0';
	return (trimmed);
}

char	*mini_getenv(char *var, char *envp[])
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (envp[i])
	{
		j = 0;
		while (envp[i][j] && envp[i][j] == var[j] && envp[i][j] != '=')
			j++;
		if (envp[i][j] == '=' && var[j] == '\0')
			return (envp[i] + j + 1);
		i++;
	}
	return (NULL);
}

/**
 * get_prompt - Creates a custom shell prompt string.
 * 
 * This function builds a prompt using the current user's name, host name, 
 * current working directory, and color formatting. It retrieves environment 
 * variables (USER, HOSTNAME, PWD,
		HOME) and uses default values if any are missing. 
 * The prompt includes user@host in a specified color and the current directory 
 * relative to the home directory.
 * 
 * Return:
 *    - A dynamically allocated string with the formatted prompt or NULL if 
 *      memory allocation fails.
 * Notes:
 *    - You must free the returned string when you are done with it.
 */
int	get_prompt(char **prompt, t_data *data)
{
	char	*user;
	char	*host;
	char	*pwd;
	int		free_host;

	user = mini_getenv("USER", data->envp);
	if (!user)
		user = "unknown_user";
	host = mini_getenv("HOSTNAME", data->envp);
	free_host = 0;
	if (!host)
	{
		host = mini_getenv("SESSION_MANAGER", data->envp);
		host = host_trim(host);
		free_host = 1;
		if (!host)
		{
			host = "unknown_host";
			free_host = 0;
		}
	}
	pwd = data->pwd;
	*prompt = malloc(sizeof(char) * (ft_strlen(user) + ft_strlen(host)
				+ ft_strlen(pwd) - ft_strlen(data->home) + 38));
	if (!*prompt)
	{
		if (free_host)
			free(host);
		return (ERROR);
	}
	ft_strcpy(*prompt, BBLUE);
	ft_strcat(*prompt, user);
	ft_strcat(*prompt, "@");
	ft_strcat(*prompt, host);
	ft_strcat(*prompt, RESET_COLOR BRIGHT_GREEN);
	ft_strcat(*prompt, " ~");
	ft_strcat(*prompt, pwd + ft_strlen(data->home));
	ft_strcat(*prompt, RESET_COLOR);
	ft_strcat(*prompt, "$\n");
	if (free_host)
		free(host);
	return (OK);
}
