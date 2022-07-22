#include "../includes/minishell.h"

extern int g_stat;

static int open_tmpfile()
{
    int fd;
    int num;
    char *file;
    char *num_str;

    num = 1;
    file = ft_strdup(".tmp");
    fd = open(file, O_WRONLY | O_CREAT | O_EXCL, 0666);
    while (fd == -1)
    {
        free(file);
        num_str = ft_itoa(num);
        file = ft_strjoin(".tmp", num_str);
        free(num_str);
        fd = open(file, O_WRONLY | O_CREAT | O_EXCL, 0666);
        num++;
    }
    tmp_files(file, ADD);
    return (fd);
}

static char *get_heredoc_str(char *here_str, char *readline)
{
    char *tmp;

    tmp = readline;
    readline = ft_strjoin(tmp, "\n");
    free(tmp);
    tmp = here_str;
    here_str = ft_strjoin(tmp, readline);
    free(tmp);
    free(readline);
    return (here_str);
}

static int get_heredoc_readend(int wrfd, char *here_str)
{
    int fd;
    char *file;

    file = tmp_files(NULL, GET)->value;
    write(wrfd, here_str, ft_strlen(here_str));
    close(wrfd);
    fd = open(file, O_RDONLY, 0666);
    free(here_str);
    if (g_stat == ETC)
    {
        tmp_files(NULL, DEL);
        close(fd);
        return (-1);
    }
    return (fd);
}

static int get_heredoc_fd(t_node *node) // 임시 파일 삭제 구현 완 아님..
{
    int fd;
    char *here_str;
    char *str;

    here_str = ft_strdup("");
    signal(SIGINT, heredoc_sig_int);
    g_stat = 0;
    fd = open_tmpfile();
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
        here_str = get_heredoc_str(here_str, str);
    }
    fd = get_heredoc_readend(fd, here_str);
    return (fd);
}

static int set_input_fd(t_node *head, t_node *file_node)
{
    if (file_node->type == INPUT)
    {
        file_node->fd[IN] = open(file_node->cmd[1], O_RDONLY);
        if (file_node->fd[IN] == -1)
        {
            printf("minishell: %s: No such file or directory\n", file_node->cmd[1]);
            g_stat = ETC;
            free_node_all(head); // free token??
            return (0);
        }
    }
    else
        file_node->fd[IN] = get_heredoc_fd(file_node);
    return (1);
}

int set_output_fd(t_node *head, t_node *file_node)
{
    if (file_node->type == APPEND)
    {
        file_node->fd[OUT] = ft_open(file_node->cmd[1], O_CREAT | O_WRONLY | O_APPEND, 0666);
        if (file_node->fd[OUT] == -1)
        {
            free_node_all(head);
            return (0);
        }
    }
    else if (file_node->type == TRUNC)
    {
        file_node->fd[OUT] = ft_open(file_node->cmd[1], O_CREAT | O_WRONLY | O_TRUNC, 0666);
        if (file_node->fd[OUT] == -1)
        {
            free_node_all(head);
            return (0);
        }
    }
    return (1);
}

int set_separator_fd(t_node *node)
{
    t_node *tmp;

    tmp = node;
    while (tmp)
    {
        if (tmp->type == INPUT || tmp->type == HEREDOC)
        {
            if (set_input_fd(node, tmp) == 0 || g_stat == ETC) // <, << node의 in에 파일 fd set
                return (0);
        }
        else if (tmp->type == TRUNC || tmp->type == APPEND) // >, >> node의 int에 파일 fd set
        {
            if (set_output_fd(node, tmp) == 0)
                return (0);
        }
        else if (tmp->type == PIPE)
        {
            pipe(tmp->fd);
            tmp->nxt->fd[IN] = tmp->fd[IN];
        }
        tmp = tmp->nxt;
    }
    return (1);
}

t_node *set_cmd_pipe(t_node *prev, t_node *tmp, t_node *cmd)
{
    prev->fd[OUT] = tmp->fd[OUT];
    if (cmd && cmd->fd[OUT] == 1) // out이 정해지지 않은 상태에서 파이프를 만나면
        cmd->fd[OUT] = tmp->fd[OUT];
    if (!cmd)
        close(tmp->fd[OUT]);
    return (NULL);
}

void set_cmd_output(t_node *tmp, t_node *cmd)
{
    if (tmp->nxt && tmp->nxt->type == PIPE)
        ft_close(tmp->nxt->fd[OUT]);
    if (cmd)
        cmd->fd[OUT] = tmp->fd[OUT];
}

t_node *init_cmd(t_node *prev, t_node *cmd)
{
    if (prev && prev->type > CMD)
        cmd->fd[IN] = prev->fd[IN];
    if (cmd->nxt == NULL)
        cmd->fd[OUT] = 1;
    return (cmd);
}

int set_cmd_fd(t_node *node)
{
    t_node *tmp;
    t_node *prev;
    t_node *cmd;

    tmp = node;
    prev = NULL;
    cmd = NULL;
    while (tmp)
    {
        if (prev && prev->type == END)
            prev = NULL;
        if (tmp->type == CMD)
            cmd = init_cmd(prev, tmp);
        if (cmd && (tmp->type == INPUT || tmp->type == HEREDOC))
            cmd->fd[IN] = tmp->fd[IN];
        if (tmp->type == TRUNC || tmp->type == APPEND)
            set_cmd_output(tmp, cmd);
        if (prev && tmp->type == PIPE)
            cmd = set_cmd_pipe(prev, tmp, cmd);
        prev = tmp;
        tmp = tmp->nxt;
    }
    return (1);
}

t_node *get_fd(t_node *node)
{
    if (!set_separator_fd(node))
        return (NULL);
    set_cmd_fd(node);
    return (node);
}