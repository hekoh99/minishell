/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hako <hako@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 17:57:51 by yubchoi           #+#    #+#             */
/*   Updated: 2022/07/23 16:58:03 by hako             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <limits.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/ioctl.h>
# include <termios.h>

# include "utils.h"

# define CMD 1
# define PIPE 2
# define TRUNC 3
# define APPEND 4
# define INPUT 5
# define HEREDOC 6
# define END 7

# define GET 0
# define ADD 1
# define DEL 2

// exit status
# define SYNTAX 258
# define CMD_NOT_FOUND 127
# define PERM 126
# define ETC 1

# define IN 0
# define OUT 1

# define SINGLE_CMD 1
# define MULTI_CMD 0

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*nxt;
}	t_env;

typedef struct s_token
{
	char			*value;
	int				type;
	struct s_token	*nxt;
	struct s_token	*prev;
}	t_token;

typedef struct s_node
{
	char			**cmd;
	int				type;
	int				fd[2];
	t_env			*envp;
	struct s_node	*nxt;
	struct s_node	*prev;
}	t_node;

typedef struct s_list
{
	char			*value;
	struct s_list	*nxt;
}	t_list;

typedef struct s_exec_arg
{
	char	**argv;
	char	*file;
	char	**envp;
}	t_exec_arg;

t_token	*trim_space(char *line);
t_token	*split_by_sep(t_token *token);
t_token	*expand(t_token *token, t_env *env);
t_token	*trim_quote(t_token *token);
void	do_trim_quote(t_token *tmp);
t_token	*add_type(t_token *token);
t_node	*exec_unit(t_token **token, t_env *envp);
t_token	*reorder_token(t_token *token);
t_token	*add_token(t_token *head, char *value);

t_token	*ft_dellist(t_token *head, char *target);
void	free_token_all(t_token *head);
void	free_node_all(t_node *head);
void	free_env_all(t_env *head);
void	free_node_all(t_node *head);
void	check_quote(char target, int *sq, int *dq);
int		check_duple_sep(char *token, int pos);
char	*inside_quote(char *value, int start, int *mid);
char	*search_env(t_env *env, char *target);

void	ft_execute(t_node *node);
void	do_execve(t_node *node);

void	ft_cd(t_node *node);
void	ft_echo(t_node *node);
t_env	*init_env(char **env);
void	ft_env(t_env *envp);
void	ft_exit(int single_cmd, t_node *node);
void	ft_export(t_node *node);
t_env	*update_env(t_env *envp, char *key, char *value);
void	ft_pwd(void);
void	ft_unset(t_node *node);
char	**ft_split(char *s, char c);
void	print_all_envp(t_env *envp, int has_prefix);
void	print_sorted_envp(t_env *envp);
t_env	*update_env(t_env *envp, char *key, char *value);
int		is_duplicate_envp(t_env *envp, char *key);
int		find_sep(char *str, int sep);
void	ft_buitlin(int single_cmd, t_node *node);
int		is_builtin(t_node *node);
int		is_single_cmd(t_node *node);
void	close_pipe(t_node *node);
void	clean_fd(void);
char	**find_path(t_node *node);
char	*find_cmd_path(char *cmd, char **paths);

t_list	*tmp_files(char *filename, int cmd);
t_list	*add_files(t_list *head, char *filename);
void	delete_files(t_list *head);
int		get_heredoc_fd(t_node *node);

int		set_input_fd(t_node *head, t_node *file_node);
int		set_output_fd(t_node *head, t_node *file_node);
int		set_separator_fd(t_node *node);

int		error_handler(t_token **token, t_token **tmp);
t_token	*open_quote_err(t_token *head);
int		print_syntax_error(t_token **token, const char *msg);

t_node	*get_fd(t_node *node);

t_node	*add_cmd_arr(t_node *new, t_token *target, int iter);

void	print_token(t_token *token, int flag);
void	print_node(t_node *node);
void	print_heredoc(t_node *node);
void	print_tmpfiles(void);
void	check_redirection(t_node *node);

void	sig_int(int signal);
void	child_sig_int(int signal);
void	heredoc_sig_int(int signal);

int		is_general_env_var(char *target, int squote);
int		is_home_env_var(char *target, int squote);

t_env *dup_env(t_env *env);
t_env *dup_envp(t_env *env);

#endif
