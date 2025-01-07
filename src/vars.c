#include "../include/minishell.h"

char *mini_getvars(char *var, t_vars *vars)
{
    t_vars *tmp;

    tmp = vars;
    while (tmp)
    {
        if (ft_strcmp(tmp->name, var) == 0)
            return (tmp->value);
        tmp = tmp->next;
    }
    return (NULL);
}

t_vars	*ft_lstnewvar(char *content)
{
    int i;
    int len_name;
    int len_value;
    char *tmp;
	t_vars	*newlist;

    len_name = 0;
    len_value = 0;
    newlist = malloc(sizeof(t_vars));
    while (content[len_name] && content[len_name] != '=')
        len_name++;
    while (content[len_value])
        len_value++;
    newlist->name = malloc(sizeof(char) * (len_name + 1));
    newlist->value = malloc(sizeof(char) * (len_value + 1));
    i = 0;
    while (content[i] && content[i] != '=')
    {
        newlist->name[i] = content[i];
        i++;
    }
    newlist->name[i] = '\0';
    tmp = ft_strdup(content + i + 1);
    newlist->value = ft_strtrim(tmp, "\"");
    newlist->next = NULL;
    return (newlist);
}

void expand_variables(char **full_cmd, char *envp[], t_data *data)
{
    char *expanded;
    char *tmp;
    char *tmp2;
    char *var;
    char *value;
    int i = 0;
    int j = 0;
    int k = 0;

    expanded = ft_calloc(sizeof(char), (strlen(*full_cmd) + 1));
    if (!expanded)
        return;
    while ((*full_cmd)[i] != '\0')
    {
        if ((*full_cmd)[i] == '$')
        {
            j = i + 1;
            while ((*full_cmd)[j] != '\0' && ft_isalnum((*full_cmd)[j]))
                j++;
            var = malloc(sizeof(char) * (j - i));
            if (!var)
            {
                free(expanded);
                return;
            }
            k = 0;
            while (i < j)
                var[k++] = (*full_cmd)[i++];
            var[k] = '\0';
            value = mini_getenv(var + 1, envp);
            if (!value)
            {   
                data->vars = NULL;
                data->vars = ft_lstnewvar("HOLA=\"fernandito\"");
                value = mini_getvars(var + 1, data->vars);
            }
            if (!value)
                value = "";
            tmp = ft_strjoin(expanded, value);
            free(expanded);
            expanded = tmp;
            free(var);
        }
        else
        {
            tmp = malloc(sizeof(char) * 2);
            if (!tmp)
            {
                free(expanded);
                return;
            }
            tmp[0] = (*full_cmd)[i];
            tmp[1] = '\0';
            tmp2 = ft_strjoin(expanded, tmp);
            free(expanded);
            free(tmp);
            expanded = tmp2;
            i++;
        }
    }
    free(*full_cmd);
    *full_cmd = expanded;
}
