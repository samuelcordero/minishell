#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#include <dirent.h>
#include <string.h>

int	ft_match(char *f_name, char *regex)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (f_name[i] && regex[j])
	{
		if (regex[j] == '*')
		{
			++j;
			while (f_name[i] && f_name[i] != regex[j])
				++i;
		}
		if (f_name[i] != regex[j])
			return (0);
		++i;
		++j;
	}
	if (f_name[i] || regex[j])
		return (0);
	return (1);
}

char	**ft_expand_wildcard(char *regex)
{
	DIR				*dir_ptr;
	struct dirent	*directory;
	char			**matches;
	int				ctr;

	dir_ptr = opendir(".");
	directory = readdir(dir_ptr);
	matches  = calloc(1024, sizeof(char *));
	ctr = -1;
	while (directory)
	{
		if (ft_match(directory->d_name, regex))
			matches[++ctr] = strdup(directory->d_name);
		directory = readdir(dir_ptr);
	}
	closedir(dir_ptr);
	return (matches);
}

/*
	TEST FOR WILDCARD EXPANSION (ONLY CURRENT DIR AS STATED IN SUBJECT)
*/
int	main(int argc, char **argv)
{
	char	**matches;
	int		i;

	i = -1;
	if (argc != 2)
		exit(1);
	printf("Before expansion: (%s)\n", argv[1]);
	matches = ft_expand_wildcard(argv[1]);
	printf("Matches:\n");
	while (matches[++i])
		printf("%s\n", matches[i]);
	return (0);
}