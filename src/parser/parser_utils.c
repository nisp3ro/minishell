/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 09:18:33 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/27 09:18:40 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_command	*initialize_command(void)
{
	t_command	*command;

	command = malloc(sizeof(t_command));
	if (!command)
		return (NULL);
	command->args = NULL;
	command->eof_count = 0;
	command->eof = NULL;
	command->append = 0;
	command->arg_count = 0;
	command->export = false;
	command->first = true;
	command->redir = NULL;
	command->next = NULL;
	return (command);
}
