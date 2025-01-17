#include "../include/minishell.h"

bool is_all_spaces(char *str)
{
    while (*str)
    {
        if (!ft_isspace(*str))
            return false;
        str++;
    }
    return true;
}

bool complete_quotes(char **full_cmd)
{
    int i;
    bool in_double_quotes;
    bool in_single_quotes;

    i = -1;
    in_double_quotes = false;
    in_single_quotes = false;
    while ((*full_cmd)[++i] != '\0')
    {
        if ((*full_cmd)[i] == '\\' && !in_single_quotes)
            i++;
        else if ((*full_cmd)[i] == '"' && !in_single_quotes)
        {
            if (in_double_quotes)
                in_double_quotes = false;
            else
                in_double_quotes = true;
        }
        else if ((*full_cmd)[i] == '\'' && !in_double_quotes)
        {
            if (in_single_quotes)
                in_single_quotes = false;
            else
                in_single_quotes = true;
        }
    }
    return !(in_double_quotes || in_single_quotes);
}


//NORMINETTE
void	sort_list(t_vars **head, t_vars *current)
{
	t_vars	*sorted;
	t_vars	*next;
	t_vars	*t;

	sorted = NULL;
	current = *head;
	while (current != NULL)
	{
		next = current->next;
		if (sorted == NULL || ft_strcmp(current->name, sorted->name) < 0)
		{
			current->next = sorted;
			sorted = current;
		}
		else
		{
			t = sorted;
			while (t->next && ft_strcmp(t->next->name, current->name) < 0)
				t = t->next;
			current->next = t->next;
			t->next = current;
		}
		current = next;
	}
	*head = sorted;
}