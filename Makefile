# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hako <hako@student.42seoul.kr>             +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/12/23 10:09:42 by hako              #+#    #+#              #
#    Updated: 2021/12/23 10:09:44 by hako             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
MINISHELL = minishell

CC = gcc 

FLAGS =	-Wall -Wextra -Werror

RM = rm -rf

SANITIZE = -g -fsanitize=address

INCS =	./includes/

OBJS =	$(SRCS:.c=.o)

CL_BOLD	 = \e[1m
CL_DIM	= \e[2m
CL_UDLINE = \e[4m

NO_COLOR = \e[0m

BG_TEXT = \e[48;2;45;55;72m
BG_BLACK = \e[48;2;30;31;33m

FG_WHITE = $(NO_COLOR)\e[0;37m
FG_TEXT = $(NO_COLOR)\e[38;2;189;147;249m
FG_TEXT_PRIMARY = $(NO_COLOR)$(CL_BOLD)\e[38;2;255;121;198m

LF = \e[1K\r$(NO_COLOR)
CRLF= \n$(LF)

all:	$(NAME)

$(NAME) :	$(OBJS)
		@make -C ./libft
		@cp ./libft/libft.a libft.a
		@$(CC) $(FLAGS) -I $(INCS) $(SRCS) main.c  libft.a -lreadline -L/Users/hako/.brew/opt/readline/lib -I/Users/hako/.brew/opt/readline/include -o $(MINISHELL)
		@printf "$(LF)🎉 $(FG_TEXT)Successfully Created $(FG_TEXT_PRIMARY)$@ $(FG_TEXT)!\n$(NO_COLOR)"

test : $(OBJS)
		@make -C ./libft
		@cp ./libft/libft.a libft.a
		@$(CC) $(FLAGS) -I $(INCS) $(SRCS) main.c  libft.a -o $(MINISHELL) $(SANITIZE)

norme:
				norminette $(SRCS) 
				norminette $(INCS)
				norminette ./libft/

clean:
				@make --no-print-directory clean -C ./libft
				@rm -f $(OBJS)
				@printf "$(LF)🧹 $(FG_TEXT)Cleaning $(FG_TEXT_PRIMARY)$(NAME)'s Object files...\n"

fclean:			clean
				@make --no-print-directory fclean -C ./libft
				@$(RM) libft.a
				@$(RM) $(NAME)
				@printf "$(LF)🧹 $(FG_TEXT)Cleaning $(FG_TEXT_PRIMARY)$(NAME)\n"

re:				fclean all

.PHONY:			all, clean, fclean, re