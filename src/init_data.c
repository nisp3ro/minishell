#include "../include/minishell.h"

/*
 * `cpy_env` creates a duplicate of the `envp` array, typically representing
 * environment variables.
 * 
 * - It calculates the number of strings in `envp` and allocates memory for 
 *   a new array `cpy`.
 * - Each string from `envp` is duplicated using `ft_strdup`. If any allocation 
 *   fails, it frees the previously allocated memory and returns `NULL`.
 * - The function returns the new array `cpy`, terminated by `NULL`, or `NULL` 
 *   on failure.
 */
char    **cpy_env(char *envp[])
{
    char **cpy;
    int i = 0;

    while (envp[i])
        i++;
    cpy = malloc(sizeof(char *) * (i + 1));
    if (!cpy)
        return (NULL);
    i = 0;
    while (envp[i])
    {
        cpy[i] = ft_strdup(envp[i]);
        if (!cpy[i])
        {
            while (i >= 0)
                free(cpy[i--]);
            free(cpy);
            return (NULL);
        }
        i++;
    }
    cpy[i] = NULL;
    return (cpy);
}

/**
 * init_data - Initializes a `t_data` structure with environment data.
 */
int    init_data(t_data *data, char *envp[])
{
    data->envp = cpy_env(envp);
    if (!data->envp)
        return (ERROR);
    data->home = NULL;
    data->pwd = getcwd(NULL, 0);
    if (!data->pwd)
        return(ERROR); //return(limpiar, ERROR);
    data->oldpwd = getcwd(NULL, 0);
    data->prompt = NULL;
    data->vars = NULL;
    return (OK);
}
