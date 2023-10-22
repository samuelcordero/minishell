/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 17:11:02 by sacorder          #+#    #+#             */
/*   Updated: 2023/10/23 00:13:15 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <stdio.h>
# include <stddef.h>
# include <limits.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 64
# endif

# ifndef OPEN_MAX
#  define OPEN_MAX 4096
# endif

typedef struct s_gnl_data
{
	char	*line;
	char	*buffer;
}	t_gnl_data;

size_t	ft_strlen_gnl(const char *str);
char	*get_next_line(int fd);
char	*ft_strjoin_gnl(char const *s1, char const *s2);
char	*ft_substr_gnl(char const *s, unsigned int start, size_t len);
char	*ft_strchr_gnl(const char*s, int c);

#endif
