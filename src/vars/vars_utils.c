/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vars_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 15:40:09 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/26 15:40:23 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	set_variable(t_vars **env_vars, char *name, char *value)
{
	t_vars	*new_var;

	new_var = malloc(sizeof(t_vars));
	if (new_var == NULL)
		return (ERROR);
	new_var->name = ft_strdup(name);
	if (new_var->name == NULL)
	{
		free(new_var);
		return (ERROR);
	}
	new_var->value = ft_strdup(value);
	if (new_var->value == NULL)
	{
		free(new_var->name);
		free(new_var);
		return (ERROR);
	}
	new_var->next = *env_vars;
	*env_vars = new_var;
	return (OK);
}
