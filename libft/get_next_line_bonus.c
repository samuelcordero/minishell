/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 17:13:12 by sacorder          #+#    #+#             */
/*   Updated: 2023/04/26 13:50:41 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static void	*ft_free(char *to_free)
{
	free(to_free);
	return (NULL);
}

static char	*ft_add_buffer(char *line, const char *buffer)
{
	char	*join;
	char	*tmp;

	tmp = ft_strchr(buffer, '\n');
	if (!tmp)
		join = ft_strjoin(line, buffer);
	else
	{
		tmp = ft_substr(buffer, 0, (tmp - buffer) + 1);
		join = ft_strjoin(line, tmp);
		if (tmp)
			tmp = ft_free(tmp);
	}
	if (line)
		free(line);
	if (join && !(*join))
		join = ft_free(join);
	return (join);
}

static char	*ft_get_buffer(int fd, int *bytes)
{
	char	*buffer;

	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (0);
	*bytes = read(fd, buffer, BUFFER_SIZE);
	if (*bytes < BUFFER_SIZE && *bytes > 0)
	{
		buffer[*bytes] = '\0';
	}
	else if (*bytes <= 0)
		buffer = ft_free(buffer);
	else
		buffer[BUFFER_SIZE] = '\0';
	return (buffer);
}

static t_gnl_data	buffer_line(int fd, char *remain)
{
	t_gnl_data	data;
	int			bytes;

	bytes = 1;
	data.line = NULL;
	data.buffer = ft_strjoin(remain, "");
	while ((bytes > 0) && data.buffer)
	{
		if (data.buffer)
		{
			data.line = ft_add_buffer(data.line, data.buffer);
			if (!ft_strchr(data.buffer, '\n'))
			{
				free(data.buffer);
				data.buffer = ft_get_buffer(fd, &bytes);
			}
			else
				break ;
		}
	}
	if (bytes == -1)
		data.line = ft_free(data.line);
	return (data);
}

char	*get_next_line(int fd)
{
	static char	*remainder[OPEN_MAX];
	t_gnl_data	data;

	if (BUFFER_SIZE <= 0 || fd < 0 || fd >= OPEN_MAX)
		return (NULL);
	data = buffer_line(fd, remainder[fd]);
	if (remainder[fd])
		remainder[fd] = ft_free(remainder[fd]);
	if (data.buffer)
	{
		remainder[fd] = ft_substr(data.buffer, (ft_strchr(data.buffer, '\n')
					- data.buffer) + 1, BUFFER_SIZE + 1);
		data.buffer = ft_free(data.buffer);
	}
	if (!data.line && remainder[fd])
		remainder[fd] = ft_free(remainder[fd]);
	return (data.line);
}
/*
int	main(void)
{
	char	*line;
	int		i;

	i = 0;
	while (i < 10){
		printf("## Type something: ##\n");
		line = get_next_line(0);
		printf("## Line %i: ##\n%s", i, line);
		free(line);
		++i;
	}
	system("leaks -q a.out");
	return(0);
}

int	main(int argc, char **argv)
{
	int		fd;
	int		i;
	char	*line;

	if (argc == 2)
		fd = open(argv[1], O_RDONLY);
	else
		fd = open("test.txt", O_RDONLY);
	printf("## File Descriptor: %i ##\n", fd);
	i = 1;
	line = get_next_line(fd);
	while (line){
		printf("## Line %i: ##\n%s", i, line);
		free(line);
		if (i == 999)
			close(fd);
		line = get_next_line(fd);
		++i;
	}
	//printf("\n#####   Ejecutando \"leaks a.out\"...   #####\n\n");
	//system("leaks a.out");
	return(0);
}
*/
/*
TEST FOR BONUS ONLY ()
in mandatory part, buffer remainder would be shared
between multiple FDs!!!!
*/
/*
int	main(void)
{
	int fd1, fd2;
	int	i;
	char *line1, *line2;
	fd1 = open("test.txt", O_RDONLY);
	fd2 = open("test2.txt", O_RDONLY);
	i = 1;
	line1 = get_next_line(fd1);
	line2 = get_next_line(fd2);
	while (line1 && line2){
		printf("## Line1 %i: ##\n%s", i, line1);
		printf("## Line2 %i: ##\n%s", i, line2);
		free(line1);
		free(line2);
		line1 = NULL;
		line2 = NULL;
		line1 = get_next_line(fd1);
		line2 = get_next_line(fd2);
		++i;
	}
	printf("\n\nEjecutando \"leaks a.out...\"\n\n");
	system("leaks a.out");
	return(0);
}*/