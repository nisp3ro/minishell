#include "../../include/minishell.h"

/**
 * @brief Trims the host string to extract a simplified hostname.
 *
 * This function searches for the first '/' character in the host string and then
 * finds the next '.' character after that. It allocates a new string containing
 * the characters between these two positions. If allocation fails, it returns NULL.
 *
 * @param host The original host string.
 * @return char* The trimmed host string, or NULL on allocation failure.
 */
static char	*host_trim(char *host)
{
	char	*trimmed;
	int		i;
	int		j;
	int		k;

	i = 0;
	j = 0;
	k = 0;
	while (host[i] != '\0' && host[i] != '/')
		i++;
	i++;
	j = i;
	while (host[j] != '\0' && host[j] != '.')
		j++;
	trimmed = malloc(sizeof(char) * (j - i + 1));
	if (!trimmed)
		return (NULL);
	while (host[i] && i < j)
		trimmed[k++] = host[i++];
	trimmed[k] = '\0';
	return (trimmed);
}

/**
 * @brief Retrieves the host name from the environment.
 *
 * This function attempts to get the host name by checking the "HOST" and "HOSTNAME"
 * environment variables. If neither is found, it then checks "SESSION_MANAGER". If
 * a value is obtained from "SESSION_MANAGER", it is trimmed using host_trim() and marked
 * for freeing by setting *free_host to true. If no host is found, it defaults to "42madrid".
 *
 * @param envp The environment variable array.
 * @param free_host Pointer to a boolean flag that is set to true if the returned host
 *                  string must be freed by the caller.
 * @return char* The host name string.
 */
char	*get_host(char **envp, bool *free_host)
{
	char	*host;

	host = mini_getenv("HOST", envp);
	if (!host)
		host = mini_getenv("HOSTNAME", envp);
	if (!host)
	{
		host = mini_getenv("SESSION_MANAGER", envp);
		if (host)
		{
			host = host_trim(host);
			if (!host)
				return (NULL);
			*free_host = true;
		}
		if (!host)
		{
			host = "42madrid";
			*free_host = false;
		}
	}
	return (host);
}

/**
 * @brief Appends the current working directory to the prompt.
 *
 * If the current directory (data->pwd) starts with the user's home directory (data->home),
 * the function appends a tilde ('~') followed by the remainder of the path to the prompt.
 * Otherwise, it appends the full current directory path.
 *
 * @param data Pointer to the minishell data structure.
 * @param prompt The prompt string being built.
 */
static void	make_pwd(t_data *data, char *prompt)
{
	if (data->home && ft_strncmp(data->pwd, data->home,
			ft_strlen(data->home)) == 0)
	{
		ft_strcat(prompt, "~");
		ft_strcat(prompt, data->pwd + ft_strlen(data->home));
	}
	else
		ft_strcat(prompt, data->pwd);
}

/**
 * @brief Constructs and prints the prompt string.
 *
 * Builds the prompt string using the user name, host, and current working directory.
 * If a Git repository is detected, it appends Git branch information to the prompt.
 * Finally, it appends a status symbol to indicate the command prompt.
 *
 * @param prompt The buffer to hold the constructed prompt string.
 * @param user The user name.
 * @param host The host name.
 * @param data Pointer to the minishell data structure.
 * @return int Returns OK on success.
 */
static int	print_prompt(char *prompt, char *user, char *host, t_data *data)
{
	bool	git_found;
	char	*name;

	name = NULL;
	ft_strcpy(prompt, user);
	ft_strcat(prompt, "@");
	ft_strcat(prompt, host);
	ft_strcat(prompt, ":"
		BRIGHT_CYAN);
	make_pwd(data, prompt);
	if (is_a_git(data, &git_found, &name) == OK && git_found)
	{
		ft_strcat(prompt, BRIGHT_BLUE);
		ft_strcat(prompt, " git:(" BRIGHT_RED);
		if (name)
			ft_strcat(prompt, name);
		ft_strcat(prompt, BRIGHT_BLUE ")");
		ft_strcat(prompt, RESET_COLOR);
	}
	ft_strcat(prompt, YELLOW " ✗ ");
	ft_strcat(prompt, RESET_COLOR);
	return (free(name), OK);
}

/**
 * @brief Retrieves and constructs the command prompt.
 *
 * This function gets the user's home directory, user name, and host name from the environment.
 * It then calls print_prompt() to build the prompt string and duplicates it using ft_strdup()
 * for use by the shell. If the host string was dynamically allocated, it frees it before returning.
 *
 * @param p Pointer to the location where the prompt string will be stored.
 * @param data Pointer to the minishell data structure.
 * @return int Returns OK on success or ERROR on failure.
 */
int	get_prompt(char **p, t_data *data)
{
	char	*user;
	char	*host;
	bool	free_host;
	char	prompt[PATH_MAX];

	data->home = mini_getenv("HOME", data->envp);
	user = mini_getenv("USER", data->envp);
	if (!user)
		user = "minishell";
	free_host = false;
	host = get_host(data->envp, &free_host);
	if (host == NULL)
		return (ERROR);
	print_prompt(prompt, user, host, data);
	*p = ft_strdup(prompt);
	if (free_host == true)
		free(host);
	return (OK);
}
