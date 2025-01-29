/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux_01.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 17:32:48 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/28 17:59:16 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_delimiter(char c)
{
	return (c == '\t' || c == ' ' || c == '|' || c == '<' || c == '>');
}

bool	is_quote(char c)
{
	if (c == '\'' || c == '\"')
		return (true);
	return (false);
}

bool	is_all_spaces(char *str)
{
	while (*str)
	{
		if (!ft_isspace(*str))
			return (false);
		str++;
	}
	return (true);
}

// bool	complete_quotes(char **full_cmd)
// {
// 	int		i;
// 	bool	in_double_quotes;
// 	bool	in_single_quotes;

// 	i = -1;
// 	in_double_quotes = false;
// 	in_single_quotes = false;
// 	while ((*full_cmd)[++i] != '\0')
// 	{
// 		if ((*full_cmd)[i] == '\\' && !in_single_quotes)
// 			i++;
// 		else if ((*full_cmd)[i] == '"' && !in_single_quotes)
// 		{
// 			if (in_double_quotes)
// 				in_double_quotes = false;
// 			else
// 				in_double_quotes = true;
// 		}
// 		else if ((*full_cmd)[i] == '\'' && !in_double_quotes)
// 		{
// 			if (in_single_quotes)
// 				in_single_quotes = false;
// 			else
// 				in_single_quotes = true;
// 		}
// 	}
// 	return (!(in_double_quotes || in_single_quotes));
// }
