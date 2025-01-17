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
void create_envp(t_data *data)
{
    char *tmp;
    data->envp = malloc(sizeof(char *) * 4);
    if (!data->envp)
        return ;
    tmp = ft_strdup("PWD=");
    data->envp[0] = ft_strjoin(tmp, getcwd(NULL, 0));
    free(tmp);
    data->envp[1] = ft_strdup("SHLVL= 0");
    data->envp[2] = ft_strdup("_=]");
    data->envp[3] = NULL;
}

void create_exp_vars(t_data *data, t_vars **exp_vars, int flag)
{
    int i;
    char *var;
    char *equal_sign;

    i = 0;
    while (data->envp[i + flag])
    {
        var = ft_strdup(data->envp[i]);
        if (!var)
            return ;//salida de error
        equal_sign = ft_strchr(var, '=');
        *equal_sign = '\0';
        equal_sign++;
        set_variable(exp_vars, var, equal_sign);
        free(var);
        i++;
    }
}

void sort_list(t_vars **head) 
{
    t_vars *sorted;
    t_vars *current;
    t_vars *next;

    sorted = NULL;
    current = *head;
    while (current != NULL) {
        t_vars *next = current->next;

        if (sorted == NULL || ft_strcmp(current->name, sorted->name) < 0) {
            current->next = sorted;
            sorted = current;
        } else {
            t_vars *temp = sorted;
            while (temp->next != NULL && ft_strcmp(temp->next->name, current->name) < 0) {
                temp = temp->next;
            }
            current->next = temp->next;
            temp->next = current;
        }

        current = next;
    }

    *head = sorted;
}

int    init_data(t_data *data, char *envp[])
{
    data->exp_vars = NULL;
    if (envp == NULL || envp[0] == NULL || envp[0][0] == '\0')
    {
        create_envp(data);
        create_exp_vars(data, &data->exp_vars, 0);
    }
    else
    {
        data->envp = cpy_env(envp);
        create_exp_vars(data, &data->exp_vars, 1);
    }
    sort_list(&data->exp_vars);
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
