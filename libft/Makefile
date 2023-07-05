# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/20 18:04:20 by sacorder          #+#    #+#              #
#    Updated: 2023/06/23 14:58:09 by sacorder         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

LIB = ar -rc
NAME = libft.a
FLAGS = -Wall -Wextra -Werror
INCLUDE = libft.h get_next_line_bonus.h ft_printf.h
SRC = ft_atoi.c ft_bzero.c ft_calloc.c ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isdigit.c ft_isprint.c ft_itoa.c ft_memchr.c ft_memcmp.c ft_memcpy.c ft_memmove.c ft_memset.c ft_putchar_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_putstr_fd.c ft_split.c ft_strchr.c ft_strdup.c ft_striteri.c ft_strjoin.c ft_strlcat.c ft_strlcpy.c ft_strlen.c ft_strmapi.c ft_strncmp.c ft_strnstr.c ft_strrchr.c ft_strtrim.c ft_substr.c ft_tolower.c ft_toupper.c
SRCBONUS = ft_lstadd_back_bonus.c ft_lstadd_front_bonus.c ft_lstclear_bonus.c ft_lstdelone_bonus.c ft_lstiter_bonus.c ft_lstlast_bonus.c ft_lstmap_bonus.c ft_lstnew_bonus.c ft_lstsize_bonus.c get_next_line_bonus.c get_next_line_utils_bonus.c ft_printf.c format_utils.c strnbr_utils.c
OBJBONUS = $(SRCBONUS:.c=.o)
OBJ = $(SRC:.c=.o)
RM=/bin/rm -f
CC=gcc

all: $(NAME)

$(NAME): $(OBJ) $(INCLUDE)
	$(LIB) $(NAME) $(OBJ)

bonus: $(OBJ) $(OBJBONUS) $(INCLUDE)
	$(LIB) $(NAME) $(OBJBONUS) $(OBJ)

%.o: %.c
	$(CC) $(FLAGS) -c -o $@ $<

clean:
	$(RM) $(OBJ)
	$(RM) $(OBJBONUS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

rebonus: fclean bonus

.PHONY: all bonus clean fclean re rebonus