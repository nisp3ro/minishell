/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 13:58:37 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/18 17:21:17 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1000
# endif

# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include "libft.h"

//gnl
char	*get_next_line(int fd);
char	*ft_read_till_n(int fd, char *save);
char	*ft_get_line(char *save);
char	*ft_subs_line_from_save(char *save);
char	*ft_free(char **str);

//moddedlbft
char	*ft_strjoin_modded(char *s1, char *s2);

//libft
size_t	ft_strlen(const char *str);
char	*ft_strchr(const char *str, int tofind);
char	*ft_substr(char const *s, unsigned int start, size_t len);

#endif
