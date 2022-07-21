#include "../includes/minishell.h"

extern int g_stat;

static int get_heredoc_fd(t_node *node) // 임시 파일 삭제 구현 완 아님..
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
    if (g_stat == ETC)
    {
        tmp_files(NULL, DEL);
        close(fd);
        return (-1);
    }
    return (fd);
}

static int set_input_fd(t_node *head, t_node *file_node, t_node *target)
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
            if (set_input_fd(node, tmp, tmp) == 0 || g_stat == ETC) // <, << node의 in에 파일 fd set
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
            if (cmd && cmd->fd[OUT] == 1) // out이 정해지지 않은 상태에서 파이프를 만나면
            {
                cmd->fd[OUT] = tmp->fd[OUT];
            }
            if (!cmd)
                close(tmp->fd[OUT]);
            cmd = NULL;
        }
        prev = tmp;
        tmp = tmp->nxt;
    }
    return (node);
}