#include "../includes/minishell.h"

extern int g_stat;

t_list *tmp_files(char *filename, int cmd)
{
    static t_list* head;

    if (cmd == GET)
        return (head);
    else if (cmd == ADD)
    {
        // printf("---> %s\n", filename);
        head = add_files(head, filename);
    }
    else if (cmd == DEL)
    {
        delete_files(head);
        head = NULL;
        return head;
    }
    return (head);
}

t_list *add_files(t_list *head, char *filename)
{
    t_list *newfile;
    t_list *tmp;

    newfile = (t_list *)malloc(sizeof(t_list));
    newfile->value = filename;
    newfile->nxt = NULL;
    if (head == NULL)
        head = newfile;
    else
    {
        tmp = head;
        while (tmp && tmp->nxt)
            tmp = tmp->nxt;
        tmp->nxt = newfile;
    }
    return (head);
}

void delete_files(t_list *head)
{
    t_list *target;
    t_list  *tmp;

    tmp = head;
    while (tmp)
    {
        target = tmp;
        unlink(target->value);
        free(target->value);
        tmp = tmp->nxt;
        free(target);
    }
}

void check_quote(char target, int *sq, int *dq)
{
    if (target == '\'' && *sq == 0 && *dq == 0)
            *sq = 1;
    else if (target == '\"' && *sq == 0 && *dq == 0)
        *dq = 1;
    else if (target == '\'' && *sq == 1)
        *sq = 0;
    else if (target == '\"' && *dq == 1)
        *dq = 0;
}

t_token *add_token(t_token *head, char *value)
{
    t_token *new;
    t_token *tmp;

    new = (t_token *)malloc(sizeof(t_token));
    new->value = value;
    new->nxt = NULL;
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

t_token *trim_space(char *line)
{
    t_token *head;
    int squote = 0;
    int dquote = 0;
    int start;
    int i = 0;

    start = 0;
    head = NULL;
    while ((line[i] != '\0' || i - start > 0))
    {
        check_quote(line[i], &squote, &dquote);
        if ((line[i] == ' ' || line[i] == '\0') && (squote == 0 && dquote == 0))
        {
            if (i - start > 0)
                head = add_token(head, ft_substr(line, start, i - start));
            start = i + 1;
            if (line[i] == '\0')
                break;
        }
        if (line[i] != '\0')
            i++;
        if (line[i] == '\0' && squote + dquote > 0)
            break;
    }
    if (squote == 1 || dquote == 1)
    {
        printf("minishell: open quotation syntax error\n");
        g_stat = SYNTAX;
        free_token_all(head);
        head = NULL;
    }
    return (head);
}

int check_duple_sep(char *token, int pos)
{
    int size;
    char *redir = "><";

    size = 1;
    if (token[pos] == token[pos + 1])
    {
        if (ft_strchr(redir, token[pos]) != 0) // >> 이나 <<
            size = 2;
        else
        {
            printf("minishell: syntax error near unexpected token '%c%c'\n", token[pos], token[pos]);
            g_stat = SYNTAX;
            size = 0;
        }
    }
    return (size);
}

t_token *do_split_by_seps(t_token *pos, int sep_size, int *index, int size) // tmp, sep_size, i, strlen
{
    t_token *seperated;

    seperated = malloc(sizeof(t_token) * sep_size);
    seperated->value = ft_substr(pos->value, *index, sep_size);

    if (pos->nxt != NULL)
    {
        // 첫번째 구분자 복사 후 리스트 병합
        seperated->nxt = pos->nxt;
        pos->nxt->prev = seperated;
        pos->nxt = seperated;
        seperated->prev = pos;
        pos = pos->nxt; // 구분자 블록
        // 구분자 후단
        seperated = malloc(sizeof(t_token));
        seperated->value = ft_substr(pos->prev->value, *index + sep_size, size);
        seperated->nxt = pos->nxt;
        pos->nxt->prev = seperated;
        pos->nxt = seperated;
        seperated->prev = pos;
        // 구분자 전단
        pos->prev->value = ft_substr(pos->prev->value, 0, *index);
        if (sep_size == 2)
            (*index)++;
    }
    else
    {
        pos->nxt = seperated;
        seperated->prev = pos;
        seperated->nxt = NULL;
        pos = pos->nxt;
        seperated = malloc(sizeof(t_token));
        seperated->value = ft_substr(pos->prev->value, *index + sep_size, size);
        pos->nxt = seperated;
        seperated->prev = pos;
        seperated->nxt = NULL;
        // 구분자 전단
        pos->prev->value = ft_substr(pos->prev->value, 0, *index);
        if (sep_size == 2)
            (*index)++;
    }
    return (pos);
}

t_token *split_by_sep(t_token *token) // 연속된 구분자도 체크 완
{
    int i;
    t_token *tmp;
    int size;
    char *sep = ";|><";
    int squote = 0;
    int dquote = 0;
    int sep_size;

    tmp = token;
    while (tmp)
    {
        i = 0;
        size = ft_strlen(tmp->value);
        while (tmp->value[i] != '\0')
        {
            check_quote(tmp->value[i], &squote, &dquote);
            if (ft_strchr(sep, tmp->value[i]) != 0 && squote == 0 && dquote == 0)
            {
                sep_size = check_duple_sep(tmp->value, i); // 0 이면 ||, ;; -> error
                if (sep_size == 0)
                {
                    free_token_all(token);
                    return (NULL);
                }
                tmp = do_split_by_seps(tmp, sep_size, &i, size);
            }
            i++;
        }
        tmp = tmp->nxt;
    }
    token = ft_dellist(token, "");
    return (token);
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

void  set_expanded_value(t_token *token, char *replaced, int start, int *index)
{
    char *head;
    char *tail;

    if (token->value[start] == '?') // status 나중에 실제값으로 대체
    {
        replaced = ft_strdup(ft_itoa(g_stat));
        (*index) = start + 1;
    }
    if (replaced != NULL)
    {
        head = ft_substr(token->value, 0, start - 1);
        tail = ft_substr(token->value, *index, ft_strlen(token->value));
        free(token->value);
        token->value = ft_strjoin(head, replaced);
        token->value = ft_strjoin(token->value, tail);
        free(replaced);
        free(head);
        free(tail);
    }
    else // NULL 일 때 - 못 찾았을 때
    {
        head = ft_substr(token->value, 0, start - 1);
        tail = ft_substr(token->value, *index, ft_strlen(token->value));
        free(token->value);
        token->value = ft_strjoin(head, tail);
        free(head);
        free(tail);
    }
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
        while (tmp->value[i] != '\0')
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

char *inside_quote(char *value, int start, int *mid)
{
    char *inside;
    char *head;
    char *tail;
    char *str;

    str = ft_substr(value, start + 1, *mid - start - 1);
    head = ft_substr(value, 0, start);
    tail = ft_substr(value, *mid + 1, ft_strlen(value));
    free(value);
    inside = ft_strjoin(head, str);
    *mid = ft_strlen(inside) - 1;
    inside = ft_strjoin(inside, tail);
    free(str);
    free(head);
    free(tail);

    return (inside);
}

t_token *trim_quote(t_token *token)
{
    t_token *tmp;
    int i;
    char *head;
    char *tail;
    char *str;
    int squote;
    int dquote;
    int start;

    tmp = token;
    while (tmp)
    {
        i = 0;
        squote = 0;
        dquote = 0;
        start = 0;
        while (tmp->value[i] != '\0')
        {
            if (tmp->value[i] == '\'' && squote == 0 && dquote == 0)
            {
                start = i;
                squote = 1;
            }
            else if (tmp->value[i] == '\"' && squote == 0 && dquote == 0)
            {
                start = i;
                dquote = 1;
            }
            else if (tmp->value[i] == '\'' && squote == 1)
            {
                squote = 0;
                tmp->value = inside_quote(tmp->value, start, &i);
            }
            else if (tmp->value[i] == '\"' && dquote == 1)
            {
                dquote = 0;
                tmp->value = inside_quote(tmp->value, start, &i);
            }
            i++;
        }
        tmp = tmp->nxt;
    }
    return (token);
}

t_node *add_node(t_node *head, t_token *target, int iter, t_env *envp)
{
    int i = 0;
    t_node *new;
    t_node *tmp;

    new = (t_node *)malloc(sizeof(t_node));
    new->type = target->type;
    new->cmd = (char **)malloc(sizeof(char *) * (iter + 1));
    new->cmd[iter] = NULL;
    new->nxt = NULL;
    new->prev = NULL;
    new->fd[IN] = 0;
    new->fd[OUT] = 1;
    new->envp = envp;
    while (i < iter)
    {
        new->cmd[i] = ft_strdup(target->value);
        target = target->nxt;
        i++;
    }
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

int get_heredoc_fd(t_node *node) // 임시 파일 삭제 구현 완
{
    int fd;
    char *here_str = ft_strdup("");
    char *str;
    char *tmp;
    char *file;
    int num;

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
    while (1)
    {
        str = readline("> ");
        if (!str)
            break;
        if (ft_strcmp(str, node->cmd[1]) == 0)
        {
            free(str);
            break;
        }
        str = ft_strjoin(str, "\n");
        tmp = here_str;
        here_str = ft_strjoin(tmp, str);
        free(tmp);
        free(str);
    }
    write(fd, here_str, ft_strlen(here_str));
    close(fd);
    fd = open(file, O_RDONLY, 0666);
    tmp_files(file, ADD); // need to delete
    /* // test code
    printf("--- heredoc  \n%s", here_str);
    // */
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

 // get_fd 최신
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
                tmp->fd[OUT] =  open(tmp->cmd[1], O_CREAT | O_WRONLY | O_APPEND, 0666);
            else if (tmp->type == TRUNC)
                tmp->fd[OUT] = open(tmp->cmd[1], O_CREAT | O_WRONLY | O_TRUNC, 0666);
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
				// printf("-->> %d\n", tmp->nxt->fd[OUT]);
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
} // */

/* // 구버전
t_node *get_fd(t_node *node)
{
    t_node *tmp;
    t_node *prev;
    t_node *target;
    int flag;

    tmp = node;
    prev = NULL;
    while (tmp)
    {
        flag = 1;
        if (tmp->type == INPUT || tmp->type == HEREDOC)
        {
            flag = 0;
            while (tmp && (tmp->type == INPUT || tmp->type == HEREDOC))
            {
                // if (tmp->infile != -1 && tmp->infile != 0)
                //     close(tmp->infile);
                if (set_input_fd(node, tmp, tmp) == 0)
                    return (0);
                prev = tmp;
                tmp = tmp->nxt; // < 다음 node 위치
            }
        }
        if (tmp && tmp->type == CMD)
        {
            flag = 0;
            target = tmp;
            if (prev && (prev->type == INPUT || prev->type == HEREDOC))
            {
                target->fd[IN] = prev->fd[IN];
                target->fd[OUT] = prev->fd[OUT];
            }
            prev = tmp;
            tmp = tmp->nxt; // target 다음
            while (tmp && (tmp->type == INPUT || tmp->type == HEREDOC))
            {
                // if (target->infile != -1 && target->infile != 0)
                //     close(target->infile);
                if (set_input_fd(node, tmp, target) == 0)
                    return (0);
                prev = tmp;
                tmp = tmp->nxt;
            } // INPUT 다음
            while (tmp && (tmp->type == TRUNC || tmp->type == APPEND))
            {
                // if (target->outfile != -1 && target->outfile != 1)
                //     close(target->outfile);
                if (tmp->type == APPEND)
                {
                    target->fd[OUT] = open(tmp->cmd[1], O_CREAT | O_WRONLY | O_APPEND, 0666);
                    tmp->fd[IN] = target->fd[OUT];
                }
                else if (tmp->type == TRUNC)
                {
                    target->fd[OUT] = open(tmp->cmd[1], O_CREAT | O_WRONLY | O_TRUNC, 0666);
                    tmp->fd[IN] = target->fd[OUT];
                }
                prev = tmp;
                tmp = tmp->nxt;
                if (tmp && tmp->type == PIPE) // > 또는 >> 이후에 바로 파이프가 나오면 파이프에 쓰지 않음
                    continue ;
            }
            if (tmp && tmp->type == PIPE) // pipe 앞 단 cmd의 out을 pipe 쓰는 쪽으로
            {
                pipe(tmp->fd);
                if (prev && (prev->type == APPEND || prev->type == TRUNC)) // redir일 경우 redir의 출력을 pipe 쓰는 쪽으로
                {
                    prev->fd[OUT] = tmp->fd[1];
                }
                else
                    target->fd[OUT] = tmp->fd[1];
            }
        }
        if (tmp && (tmp->type == PIPE)) // 파이프 뒷 단 cmd의 in에 pipe 읽는 부분 연결
        {
            // if (tmp->fd[OUT] == 1)
            //     pipe(tmp->fd);
            if (tmp->nxt)
                tmp->nxt->fd[IN] = tmp->fd[0];
        }
        if (flag)
        {
            prev = tmp;
            tmp = tmp->nxt;
        }
    }
    return (node);
} // */

t_node *exec_unit(t_token **token, t_env *envp)
{
    t_node *head;
    t_token *token_head;
    t_token *tmp;
    t_token *start;
    int i = 0;

    head = NULL;
    tmp = *token;
    token_head = *token;
    while (tmp)
    {
        i = 0;
        start = tmp;
        if (tmp->type == CMD)
        {
            while (tmp && tmp->type == CMD)
            {
                tmp = tmp->nxt;
                i++;
            }
            head = add_node(head, start, i, envp);
        }
        else if (tmp->type == PIPE)
        {
            if (!tmp->prev || tmp->prev->type != CMD)
            {
                printf("minishell: syntax error near unexpected token `|'\n");
                g_stat = SYNTAX;
                free_token_all(token_head);
                *token = NULL;
                return (0);
            }
            if (!tmp->nxt || tmp->nxt->type == PIPE)
            {
                printf("minishell: syntax error near unexpected token `|'\n");
                g_stat = SYNTAX;
                free_token_all(token_head);
                *token = NULL;
                return (0);
            }
            head = add_node(head, start, 1, envp);
            tmp = tmp->nxt;
        }
        else if (tmp->type == END)
        {
            if (!tmp->prev || tmp->prev->type != CMD)
            {
                printf("minishell: syntax error near unexpected token `;'\n");
                g_stat = SYNTAX;
                free_token_all(token_head);
                *token = NULL;
                return (0);
            }
            if (tmp->nxt && tmp->nxt->type == END)
            {
                printf("minishell: syntax error near unexpected token `;'\n");
                g_stat = SYNTAX;
                free_token_all(token_head);
                *token = NULL;
                return (0);
            }
            head = add_node(head, start, 1, envp);
            tmp = tmp->nxt;
        }
        else if (tmp->type == TRUNC || tmp->type == APPEND || tmp->type == INPUT || tmp->type == HEREDOC)
        {
            if (!tmp->nxt)
            {
                printf("minishell: syntax error near unexpected token `newline'\n");
                g_stat = SYNTAX;
                free_token_all(token_head);
                *token = NULL;
                return (0);
            }
            if (tmp->nxt->type > 1)
            {
                printf("minishell: syntax error near unexpected token `%s'\n", tmp->nxt->value);
                g_stat = SYNTAX;
                free_token_all(token_head);
                *token = NULL;
                return (0);
            }
            head = add_node(head, start, 2, envp);
            tmp = tmp->nxt;
            tmp = tmp->nxt;
        }
        if (!tmp)
            break;
    }
    head = get_fd(head);
    return (head);
}