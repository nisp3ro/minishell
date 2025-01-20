/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_history.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 15:12:06 by jvidal-t          #+#    #+#             */
/*   Updated: 2025/01/20 17:12:40 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

set_matrix_null(char **matrix)
{
    
}

void delete_history_file(t_data *data)
{
    rl_clear_history();
    set_matrix_null(data->history);
}

void print_history(t_data *data)
{
    char *line;

    line = get_next_line(data->fd);
    while (line)
    {
        write(1, line, ft_strlen(line));

        if (*line != '\0')
            write(1, "\n", 1);
        free(line);
        line = get_next_line(data->fd);
    }
}
