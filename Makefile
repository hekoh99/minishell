NAME = minishell

UTILS = srcs/utils
BUILTIN = srcs/builtin
EXECUTE = srcs/execute
SIGNALS = srcs/signals
FD = srcs/fd
TOKEN = srcs/token
PARSE = srcs/parse_utils

SRCS		= $(UTILS)/ft_strdup.c \
	$(UTILS)/ft_strlen.c \
	$(UTILS)/ft_substr.c \
	$(UTILS)/ft_isalphadigit.c \
	$(UTILS)/ft_system_call.c \
	$(UTILS)/ft_system_call2.c \
	$(UTILS)/builtin_utils.c \
	$(UTILS)/ft_strchr.c \
	$(UTILS)/ft_strncmp.c \
	$(UTILS)/token_free.c \
	$(UTILS)/ft_strjoin.c \
	$(UTILS)/ft_strlcpy.c \
	$(UTILS)/ft_itoa.c \
	$(UTILS)/ft_atoi.c \
	$(UTILS)/ft_split.c \
	$(UTILS)/free_matrix.c \
	$(UTILS)/error.c \
	$(BUILTIN)/cd.c \
	$(BUILTIN)/echo.c \
	$(BUILTIN)/env.c \
	$(BUILTIN)/exit.c \
	$(BUILTIN)/export.c \
	$(BUILTIN)/export_util.c \
	$(BUILTIN)/export_util2.c \
	$(BUILTIN)/pwd.c \
	$(BUILTIN)/unset.c \
	$(EXECUTE)/execute.c \
	$(EXECUTE)/execute_util.c \
	$(EXECUTE)/execve.c \
	$(EXECUTE)/execve_util.c \
	$(TOKEN)/token.c \
	$(SIGNALS)/signal.c \
	$(FD)/heredoc_tmp_files.c \
	$(PARSE)/parse_error.c \
	$(TOKEN)/trim.c \
	$(TOKEN)/split_by_sep.c \
	$(PARSE)/parse_check.c \
	$(TOKEN)/expand.c \
	$(PARSE)/search_env.c \
	$(FD)/fd.c \
	$(TOKEN)/exec_unit.c \
	$(FD)/heredoc_fd.c \
	$(FD)/file_fd.c \
	$(TOKEN)/reorder_token.c \
	$(BUILTIN)/init_env.c \
	$(UTILS)/define_env_category.c \
	$(BUILTIN)/export_util3.c \

OBJS		= $(SRCS:%.c=%.o)

CC = cc
CFLAGS = -Werror -Wall -Wextra

BREW_DIR		:= $(shell brew --prefix readline)
READLINE_LIB	:= $(addprefix $(BREW_DIR)/, lib)
READLINE_INC	:= -I $(addprefix $(BREW_DIR)/, include)
READLINE_LIB	:= -lreadline -L$(READLINE_LIB)

.PHONY	:all
all		:$(NAME)

$(NAME)	:$(OBJS)
	$(CC) -o $(NAME) main.c $(OBJS) $(READLINE_LIB) $(READLINE_INC)

%.o: %.c
	$(CC) $(CFLAGS) $(READLINE_INC) -c $< -o $@

.PHONY	:clean
clean	:
	rm -f $(OBJS)

.PHONY	:fclean
fclean	:clean
	rm -f $(NAME)

.PHONY	:re
re		:$(MAKE) fclean
	$(MAKE) all
