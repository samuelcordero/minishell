# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/05 13:14:44 by sacorder          #+#    #+#              #
#    Updated: 2023/07/05 17:17:39 by sacorder         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

LIBFT = libft/libft.a
LIB = -L libft -lft -lreadline
NAME = minishell
FLAGS = -Wall -Wextra -Werror
INCLUDE = inc/fdf.h 
SRC = src/minishell.c src/lexer.c
OBJ = $(SRC:.c=.o)
RM=/bin/rm -f
CC = gcc

all: $(NAME)

$(NAME): $(LIBFT) $(OBJ)
	$(CC) $(FLAGS) -o $(NAME) $(OBJ) $(LIB)

$(LIBFT):
	@make bonus -C ./libft

%.o: %.c
	$(CC) $(FLAGS) -c -o $@ $<

clean:
	@make clean -C ./libft
	$(RM) $(OBJ)
	$(RM) $(OBJBONUS)

fclean:
	@make fclean -C ./libft
	$(RM) $(NAME)
	$(RM) $(OBJ)
	$(RM) $(OBJBONUS)

re: fclean all

rebonus: fclean bonus

.PHONY: all clean fclean re