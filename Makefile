# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/05 13:14:44 by sacorder          #+#    #+#              #
#    Updated: 2023/10/02 16:21:47 by sacorder         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

LIBFT = libft/libft.a
LDFLAGS = -L libft -lft -lreadline
NAME = minishell
CFLAGS = -Wall -Wextra -Werror -O3 -Iinc
INCLUDE = inc/fdf.h 
SRC =	src/minishell.c \
		src/lexer.c \
		src/lexer_utils.c \
		src/expander.c \
		src/init.c \
		src/redirect_utils.c \
		src/redirect_utils2.c
OBJ = $(SRC:.c=.o)
RM=/bin/rm -f
CC = gcc

all: $(NAME)

$(NAME): $(LIBFT) $(OBJ)
	$(CC) -o $(NAME) $(OBJ) $(LDFLAGS)

$(LIBFT):
	@make bonus -C ./libft

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@make clean -C ./libft
	$(RM) $(OBJ)
	$(RM) $(OBJBONUS)

fclean:
	@make fclean -C ./libft
	$(RM) $(NAME)
	$(RM) $(OBJ)
	$(RM) $(OBJBONUS)

asan:: CFLAGS += -g3 -fsanitize=address
asan:: LDFLAGS += -g3 -fsanitize=address
asan:: re

re:: fclean 
re:: all

rebonus: fclean bonus

.PHONY: all clean fclean re
