/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubchoi <yubchoi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/20 16:28:58 by hako              #+#    #+#             */
/*   Updated: 2022/07/21 14:15:32 by yubchoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../includes/minishell.h"

extern int g_stat;

t_token *add_token(t_token *head, char *value)
{
    t_token *new;
    t_token *tmp;

    new = (t_token *)malloc(sizeof(t_token));
    new->value = value;
    new->nxt = NULL;
    new->prev = NULL;
    if (head == NULL)
    {
        head = new;
        head->prev = NULL;
        tmp = head;
    }
    else
    {
        tmp = head;
        while (tmp->nxt)
            tmp = tmp->nxt;
        tmp->nxt = new;
        new->prev = tmp;
    }
    return (head);
}

char *search_env(t_env *env, char *target)
{
    char *real_val;
    int i;
    int find;

    real_val = NULL;
    while (env)
    {
        if (ft_strncmp(env->key, target, select_longer(env->key, target)) == 0) // 환경변수 찾음
        {
            real_val = ft_substr(env->value, 0, ft_strlen(env->value));
            break;
        }
        env = env->nxt;
    }
    return (real_val);
}

void set_expanded_value(t_token *token, char *replaced, int start, int *index)
{
    char *head;
    char *tail;

    if (token->value[start] == '?') // status 실제값으로 대체 완
    {
        replaced = ft_strdup(ft_itoa(g_stat));
        (*index) = start + 1;
    }
    if (!replaced)
        replaced = ft_strdup("");
    head = ft_substr(token->value, 0, start - 1);
    tail = ft_substr(token->value, *index, ft_strlen(token->value));
    free(token->value);
    token->value = ft_strjoin(head, replaced);
    token->value = ft_strjoin(token->value, tail);
    free(replaced);
    free(head);
    free(tail);
    if (ft_strlen(token->value) == 0) // 환경변수가 없으며 출력할 문자도 없을 때
    {
        free(token->value);
        token->value = NULL;
    }
    (*index) = (*index) - ((*index) - start + 1);
}

t_token *expand(t_token *token, t_env *env) // parse $ ~ 작은 따옴표 안은 무시
{
    int i;
    int squote;
    int dquote;
    int start;
    char *target;
    char *replaced;
    t_token *tmp;

    tmp = token;
    while (tmp)
    {
        i = 0;
        squote = 0;
        dquote = 0;
        while (tmp->value && tmp->value[i] != '\0')
        {
            check_quote(tmp->value[i], &squote, &dquote);
            if (tmp->value[i] == '$' && tmp->value[i + 1] != '\0' && squote == 0)
            {
                start = i + 1;
                i++;
                while ((ft_isalpha(tmp->value[i]) == 1 || ft_isdigit(tmp->value[i])) && tmp->value[i] != '\0')
                    i++;
                target = ft_substr(tmp->value, start, i - start);
                replaced = search_env(env, target);
                free(target);
                set_expanded_value(tmp, replaced, start, &i);
                // i--; // 반복문 후 i는 구분자 위치 또는 문자열의 끝에 위치
            }
            else if (i == 0 && tmp->value[i] == '~' && (ft_strlen(tmp->value) == 1 || tmp->value[i + 1] == '/') && squote == 0)
            {
                replaced = search_env(env, "HOME");
                if (!replaced) // HOME 없을 때
                    replaced = ft_strdup(getenv("HOME"));
                i++;
                set_expanded_value(tmp, replaced, 1, &i);
            }
            else
                i++;
        }
        tmp = tmp->nxt;
    }
    return (token);
}

t_token *add_type(t_token *token)
{
    t_token *tmp;

    tmp = token;
    while (tmp)
    {
        if (tmp->value[0] == '>')
        {
            tmp->type = TRUNC;
            if (tmp->value[1] == '>')
                tmp->type = APPEND;
        }
        else if (tmp->value[0] == '<')
        {
            tmp->type = INPUT;
            if (tmp->value[1] == '<')
                tmp->type = HEREDOC;
        }
        else if (tmp->value[0] == '|')
            tmp->type = PIPE;
        else if (tmp->value[0] == ';')
            tmp->type = END;
        else
            tmp->type = CMD;
        tmp = tmp->nxt;
    }
    return (token);
}

t_node *new_node(int type, int size, t_env *envp)
{
    t_node *new;

    new = (t_node *)malloc(sizeof(t_node));
    new->type = type;
    new->cmd = (char **)malloc(sizeof(char *) * (size + 1));
    new->cmd[size] = NULL;
    new->nxt = NULL;
    new->prev = NULL;
    new->fd[IN] = 0;
    new->fd[OUT] = 1;
    new->envp = envp;
    return (new);
}

t_node *add_cmd_arr(t_node *new, t_token *target, int iter)
{
    int i;
    int j;

    i = 0;
    j = 0;
    while (i < iter)
    {
        if (target->value != NULL)
        {
            new->cmd[j] = ft_strdup(target->value);
            j++;
        }
        target = target->nxt;
        i++;
    }
    new->cmd[j] = NULL;
    return (new);
}

t_node *add_node(t_node *head, t_token *target, int iter, t_env *envp)
{
    t_node *new;
    t_node *tmp;

    new = new_node(target->type, iter, envp);
    new = add_cmd_arr(new, target, iter);
    if (head == NULL)
        head = new;
    else
    {
        tmp = head;
        while (tmp->nxt)
            tmp = tmp->nxt;
        tmp->nxt = new;
        new->prev = tmp;
    }
    return (head);
}

int get_heredoc_fd(t_node *node) // 임시 파일 삭제 구현 완 아님..
{
    int fd;
    char *here_str = ft_strdup("");
    char *str;
    char *tmp;
    char *file;
    int num;

    signal(SIGINT, heredoc_sig_int);
    num = 1;
    file = ft_strdup("tmp");
    fd = open(file, O_WRONLY | O_CREAT | O_EXCL, 0666);
    while (fd == -1)
    {
        free(file);
        file = ft_strjoin("tmp", ft_itoa(num));
        fd = open(file, O_WRONLY | O_CREAT | O_EXCL, 0666);
        num++;
    }
    g_stat = 0;
    while (g_stat != ETC)
    {
        str = readline("> ");
        if (!str)
            break;
        if (ft_strcmp(str, node->cmd[1]) == 0)
        {
            free(str);
            break;
        }
        tmp = str;
        str = ft_strjoin(str, "\n");
        free(tmp);
        tmp = here_str;
        here_str = ft_strjoin(tmp, str);
        free(tmp);
        free(str);
    }
    write(fd, here_str, ft_strlen(here_str));
    close(fd);
    fd = open(file, O_RDONLY, 0666);
    tmp_files(file, ADD);
    free(here_str);
    return (fd);
}

int set_input_fd(t_node *head, t_node *file_node, t_node *target)
{
    if (file_node->type == INPUT)
    {
        target->fd[IN] = open(file_node->cmd[1], O_RDONLY);
        if (target->fd[IN] == -1)
        {
            printf("minishell: %s: No such file or directory\n", file_node->cmd[1]);
            g_stat = ETC;
            free_node_all(head); // free token??
            return (0);
        }
    }
    else
        target->fd[IN] = get_heredoc_fd(file_node);
    return (1);
}

t_node *get_fd(t_node *node)
{
    t_node *tmp;
    t_node *prev;
    t_node *cmd;

    tmp = node;
    prev = NULL;
    while (tmp)
    {
        if (tmp->type == INPUT || tmp->type == HEREDOC)
        {
            if (set_input_fd(node, tmp, tmp) == 0) // <, << node의 in에 파일 fd set
                return (0);
        }
        else if (tmp->type == TRUNC || tmp->type == APPEND) // >, >> node의 int에 파일 fd set
        {
            if (tmp->type == APPEND)
            {
                tmp->fd[OUT] = ft_open(tmp->cmd[1], O_CREAT | O_WRONLY | O_APPEND, 0666);
                if (tmp->fd[OUT] == -1)
                {
                    free_node_all(node);
                    return (0);
                }
            }
            else if (tmp->type == TRUNC)
            {
                tmp->fd[OUT] = ft_open(tmp->cmd[1], O_CREAT | O_WRONLY | O_TRUNC, 0666);
                if (tmp->fd[OUT] == -1)
                {
                    free_node_all(node);
                    return (0);
                }
            }
        }
        else if (tmp->type == PIPE)
        {
            pipe(tmp->fd);
            tmp->nxt->fd[IN] = tmp->fd[IN];
        }
        prev = tmp;
        tmp = tmp->nxt;
    }
    tmp = node;
    prev = NULL;
    cmd = NULL;
    while (tmp)
    {
        if (prev && prev->type == END)
            prev = NULL;
        if (tmp->type == CMD)
        {
            cmd = tmp;
            if (prev && prev->type > CMD)
            {
                tmp->fd[IN] = prev->fd[IN];
                tmp->fd[OUT] = prev->fd[OUT];
            }
            if (tmp->nxt == NULL)
                tmp->fd[OUT] = 1;
        }
        if (cmd && (tmp->type == INPUT || tmp->type == HEREDOC))
        {
            cmd->fd[IN] = tmp->fd[IN];
        }
        if (tmp->type == TRUNC || tmp->type == APPEND)
        {
            if (tmp->nxt && tmp->nxt->type == PIPE)
            {
                ft_close(tmp->nxt->fd[OUT]);
            }
            if (cmd)
            {
                cmd->fd[OUT] = tmp->fd[OUT];
            }
        }
        if (prev && tmp->type == PIPE)
        {
            prev->fd[OUT] = tmp->fd[OUT];
            if (cmd->fd[OUT] == 1) // out이 정해지지 않은 상태에서 파이프를 만나면
            {
                cmd->fd[OUT] = tmp->fd[OUT];
            }
            cmd = NULL;
        }
        prev = tmp;
        tmp = tmp->nxt;
    }
    return (node);
}

void connect_cmd(t_token **head, t_token *cmd, t_token *arg_start, t_token *redir)
{
    if (cmd)
    {
        cmd->nxt = arg_start;
        arg_start->prev = cmd;
    }
    else // cmd가 null 일때
    {
        if (!redir->prev)
            *head = arg_start;
        else
            redir->prev->nxt = arg_start;
    }
}

t_token *do_reorder_token(t_token **head, t_token *file, t_token *redir, t_token *cmd)
{
    t_token *arg_start;
    t_token *arg_end;
    t_token *tmp;

    arg_start = file->nxt;
    tmp = arg_start;
    while (tmp && tmp->type == CMD)
    {
        arg_end = tmp;
        tmp = tmp->nxt;
    }
    file->nxt = tmp; // redir의 뒷단을 tmp에 연결
    if (tmp)
        tmp->prev = file;
    connect_cmd(head, cmd, arg_start, redir);
    arg_end->nxt = redir;
    redir->prev = arg_end;
    return (arg_end);
}

t_token *reorder_token(t_token *token)
{
    t_token *tmp;
    t_token *cmd;
    t_token *redir;
    t_token *file;

    tmp = token;
    cmd = NULL;
    while (tmp)
    {
        if (tmp->type == CMD) // 명령어
            cmd = tmp;
        else if (tmp->type == PIPE || tmp->type == END)
            cmd = NULL;
        else // redir 일 때
        {
            redir = tmp;
            file = tmp->nxt;
            if (!file)
                print_syntax_error(&token, "minishell: syntax error near unexpected token `newline'\n");
            if (file && file->nxt && file->nxt->type == CMD) // redir 뒷 단이 cmd면
                tmp = do_reorder_token(&token, file, redir, cmd);
        }
        tmp = tmp->nxt;
    }
    return (token);
}

t_node *add_node_by_type(t_node *head, t_token **token, t_token **tmp, t_env *envp)
{
    t_token *start;
    t_node *return_head;

    start = *tmp;
    if (*token)
    {
        return_head = error_handler(head, token, tmp);
        if (!return_head)
        {
            free_node_all(head);
            return (NULL);
        }
    }
    if ((*tmp)->type == PIPE)
        return_head = add_node(head, start, 1, envp);
    else if ((*tmp)->type == END)
        return_head = add_node(head, start, 1, envp);
    else if ((*tmp)->type == TRUNC || (*tmp)->type == APPEND || (*tmp)->type == INPUT || (*tmp)->type == HEREDOC)
    {
        return_head = add_node(head, start, 2, envp);
        (*tmp) = (*tmp)->nxt;
    }
    (*tmp) = (*tmp)->nxt;
    return (return_head);
}

t_node *get_exec_unit(t_node *head, t_token **token, t_token **tmp, t_env *envp)
{
    int i;
    t_token *start;

    i = 0;
    start = *tmp;
    if ((*tmp)->type == CMD)
    {
        while ((*tmp) && (*tmp)->type == CMD)
        {
            (*tmp) = (*tmp)->nxt;
            i++;
        }
        head = add_node(head, start, i, envp);
    }
    else
    {
        head = add_node_by_type(head, token, tmp, envp);
        if (!head)
            return (NULL);
    }
    return (head);
}

t_node *exec_unit(t_token **token, t_env *envp)
{
    t_node *head;
    t_token *tmp;

    head = NULL;
    tmp = *token;
    while (tmp)
    {
        head = get_exec_unit(head, token, &tmp, envp);
        if (!head)
            return (NULL);
    }
    head = get_fd(head);
    return (head);
}