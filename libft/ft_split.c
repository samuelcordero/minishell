/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/09 13:27:00 by sacorder          #+#    #+#             */
/*   Updated: 2023/03/16 15:37:49 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

static size_t	ft_count_words(char const *s, char c)
{
	unsigned int	counter;
	unsigned int	position;

	counter = 0;
	position = 0;
	if (!s)
		return (0);
	while (s[position] != '\0')
	{
		if (s[position] == c)
		{
			while (s[position] == c && s[position])
				++position;
		}
		if (s[position] != '\0' && s[position] != c)
		{
			++counter;
			while (s[position] != c && s[position])
				++position;
		}
	}
	return (counter);
}

static size_t	ft_get_wordatpos_len(char const *s, char c, size_t position)
{
	size_t	result;
	size_t	word_counter;
	size_t	string_pos;

	word_counter = 0;
	string_pos = 0;
	result = 0;
	while (s[string_pos] != '\0' && word_counter <= position)
	{
		while (c == s[string_pos])
			++string_pos;
		if (word_counter == position)
		{
			while (c != s[string_pos] && s[string_pos] != '\0')
			{
				++result;
				++string_pos;
			}
		}
		while (c != s[string_pos] && s[string_pos] != '\0')
			++string_pos;
		++word_counter;
	}
	return (result);
}

static char	*ft_get_word_pos(char const *s, char c, size_t position)
{
	char	*result;
	size_t	word_counter;
	size_t	string_pos;
	size_t	result_pos;

	word_counter = 0;
	result_pos = 0;
	string_pos = 0;
	result = (char *) malloc(sizeof(char) *(ft_get_wordatpos_len(s,
					c, position) + 1));
	if (!result)
		return (result);
	while (s[string_pos] != '\0' && word_counter <= position)
	{
		while (c == s[string_pos])
			++string_pos;
		if (word_counter == position)
			while (c != s[string_pos] && s[string_pos] != '\0')
				result[result_pos++] = s[string_pos++];
		while (c != s[string_pos] && s[string_pos] != '\0')
			++string_pos;
		++word_counter;
	}
	result[result_pos] = '\0';
	return (result);
}

char	**ft_split(char const *s, char c)
{
	char	**result;
	size_t	word_nb;
	size_t	position;

	word_nb = ft_count_words(s, c);
	result = (char **) malloc(sizeof(char *) * (word_nb + 1));
	position = 0;
	if (!result)
		return (result);
	while (position < word_nb)
	{
		result[position] = ft_get_word_pos(s, c, position);
		++position;
	}
	result[position] = NULL;
	return (result);
}

/*
#include <stdio.h>
int	main(int argc, char **argv)
{
	char **res;

	res = ft_split(argv[1], argv[2][0]);	
	printf("Word number: %d\n", ft_count_words(argv[1], argv[2][0]));
	while (*res)
	{	
		printf("Word: %s\n", *res);
		++res;
	}
}*/
