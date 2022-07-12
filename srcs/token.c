#include "../includes/minishell.h"

t_token *trim_space(char *line)
{
    t_token *head;
    t_token *new;
    t_token *tmp;
    int squote = 0;
    int dquote = 0;
    int start;
    int i = 0;

    start = 0;
    head = NULL;
    while ((line[i] != '\0' || i - start > 0))
    {
        if (line[i] == '\'' && squote == 0 && dquote == 0)
            squote = 1;
        else if (line[i] == '\"' && squote == 0 && dquote == 0)
            dquote = 1;
        else if (line[i] == '\'' && squote == 1)
            squote = 0;
        else if (line[i] == '\"' && dquote == 1)
            dquote = 0;
        if ((line[i] == ' ' || line[i] == '\0') && (squote == 0 && dquote == 0))
        {
            if (i - start > 0)
            {
                new = malloc(sizeof(t_token));
                new->value = ft_substr(line, start, i - start);
                new->nxt = NULL;
                if (head == NULL)
                {
                    head = new;
                    head->prev = NULL;
                    tmp = head;
                }
                else
                {
                    tmp->nxt = new;
                    new->prev = tmp;
                    tmp = new;
                }
            }
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
        printf("open quotation error\n");
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
            size = 0;
        }
    }
    return (size);
}

t_token *split_by_sep(t_token *token) // 연속된 구분자도 체크 완
{
    int i;
    t_token *seperated;
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
            if (tmp->value[i] == '\'' && squote == 0 && dquote == 0)
                squote = 1;
            else if (tmp->value[i] == '\"' && squote == 0 && dquote == 0)
                dquote = 1;
            else if (tmp->value[i] == '\'' && squote == 1)
                squote = 0;
            else if (tmp->value[i] == '\"' && dquote == 1)
                dquote = 0;
            if (ft_strchr(sep, tmp->value[i]) != 0 && squote == 0 && dquote == 0)
            {
                sep_size = check_duple_sep(tmp->value, i); // 0 이면 ||, ;; -> error
                if (sep_size == 0)
                {
                    free_token_all(token);
                    return (NULL);
                }
                seperated = malloc(sizeof(t_token) * sep_size);
                seperated->value = ft_substr(tmp->value, i, sep_size);
                if (tmp->nxt != NULL)
                {
                    // 첫번째 구분자 복사 후 리스트 병합
                    seperated->nxt = tmp->nxt;
                    tmp->nxt->prev = seperated;
                    tmp->nxt = seperated;
                    seperated->prev = tmp;
                    tmp = tmp->nxt; // 구분자 블록
                    // 구분자 후단
                    seperated = malloc(sizeof(t_token));
                    seperated->value = ft_substr(tmp->prev->value, i+sep_size, size);
                    seperated->nxt = tmp->nxt;
                    tmp->nxt->prev = seperated;
                    tmp->nxt = seperated;
                    seperated->prev = tmp;
                    // 구분자 전단
                    tmp->prev->value = ft_substr(tmp->prev->value, 0, i);
                    if (sep_size == 2)
                        i++;
                }
                else
                {
                    tmp->nxt = seperated;
                    seperated->prev = tmp;
                    seperated->nxt = NULL;
                    tmp = tmp->nxt;
                    seperated = malloc(sizeof(t_token));
                    seperated->value = ft_substr(tmp->prev->value, i+sep_size, size);
                    tmp->nxt = seperated;
                    seperated->prev = tmp;
                    seperated->nxt = NULL;
                    // 구분자 전단
                    tmp->prev->value = ft_substr(tmp->prev->value, 0, i);
                    if (sep_size == 2)
                        i++;
                }
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
        if (ft_strncmp(env->key, target, ft_strlen(env->key)) == 0) // 환경변수 찾음
        {
            real_val = ft_substr(env->value, 0, ft_strlen(env->value));
            break;
        }
        env = env->nxt;
    }
    return (real_val);
}

t_token *expand(t_token *token, t_env *env) // parse $ ~ 작은 따옴표 안은 무시
{
    int i;
    int squote;
    int dquote;
    int start;
    char *target;
    char *replaced;
    char *sep = "$ ~\'\"";
    char *head;
    char *tail;
    t_token *tmp;

    tmp = token;
    while (tmp)
    {
        i = 0;
        squote = 0;
        dquote = 0;
        while (tmp->value[i] != '\0')
        {
            if (tmp->value[i] == '\'' && squote == 0 && dquote == 0)
                squote = 1;
            else if (tmp->value[i] == '\"' && squote == 0 && dquote == 0)
                dquote = 1;
            else if (tmp->value[i] == '\'' && squote == 1)
                squote = 0;
            else if (tmp->value[i] == '\"' && dquote == 1)
                dquote = 0;
            if (tmp->value[i] == '$' && squote == 0)
            {
                start = i + 1;
                i++;
                while (ft_strchr(sep, tmp->value[i]) == 0 && tmp->value[i] != '\0')    
                    i++;
                target = ft_substr(tmp->value, start, i - start);
                replaced = search_env(env, target);
                free(target);

                if(tmp->value[start] == '?') // status 나중에 실제값으로 대체
                {
                    int status = 0;
                    replaced = ft_strdup(ft_itoa(status));
                    i = start + 1;
                }
                if (replaced != NULL)
                {
                    head = ft_substr(tmp->value, 0, start - 1);
                    tail = ft_substr(tmp->value, i, ft_strlen(tmp->value));
                    free(tmp->value);
                    tmp->value = ft_strjoin(head, replaced);
                    tmp->value = ft_strjoin(tmp->value, tail);
                    free(replaced);
                    free(head);
                    free(tail);
                }
                else // NULL 일 때 - 못 찾았을 때
                {
                    head = ft_substr(tmp->value, 0, start - 1);
                    tail = ft_substr(tmp->value, i, ft_strlen(tmp->value));
                    free(tmp->value);
                    tmp->value = ft_strjoin(head, tail);
                    free(head);
                    free(tail);
                }
                i--; // 반복문 후 i는 구분자 위치 또는 문자열의 끝에 위치
            }
            if (tmp->value[i] == '~' && (ft_strlen(tmp->value) == 1 || tmp->value[i + 1] == '/') && squote == 0)
            {
                replaced = search_env(env, "HOME");
                tail = ft_substr(tmp->value, i + 1, ft_strlen(tmp->value));
                free(tmp->value);
                tmp->value = ft_strjoin(replaced, tail);
                free(tail);
            }
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
    while(tmp)
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
                str = ft_substr(tmp->value, start + 1, i - start - 1);
                head = ft_substr(tmp->value, 0, start);
                tail = ft_substr(tmp->value, i + 1, ft_strlen(tmp->value));
                free(tmp->value);
                tmp->value = ft_strjoin(head, str);
                i = ft_strlen(tmp->value) - 1;
                tmp->value = ft_strjoin(tmp->value, tail);
                free(str);
                free(head);
                free(tail);
            }
            else if (tmp->value[i] == '\"' && dquote == 1)
            {
                dquote = 0;
                str = ft_substr(tmp->value, start + 1, i - start - 1);
                head = ft_substr(tmp->value, 0, start);
                tail = ft_substr(tmp->value, i + 1, ft_strlen(tmp->value));
                free(tmp->value);
                tmp->value = ft_strjoin(head, str);
                i = ft_strlen(tmp->value) - 1;
                tmp->value = ft_strjoin(tmp->value, tail);
                free(str);
                free(head);
                free(tail);
            }
            i++;
        }
        tmp = tmp->nxt;
    }
    return (token);
}

t_node *add_node(t_node *head, t_token *target, int iter, int type)
{
    int i = 0;
    t_node *new;
    t_node *tmp;

    new = (t_node *)malloc(sizeof(t_node));
    new->type = type;
    new->cmd = (char **)malloc(sizeof(char *) * (iter + 1));
    new->cmd[iter] = NULL;
    new->nxt = NULL;
    new->infile = 0;
    new->outfile = 1;
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
    }
    return (head);
}

int get_heredoc_fd(t_node *node) // EOF 처리도 해야함
{
    int fd[2];
    char *here_str = ft_strdup("");
    char *str;
    char *tmp;

    pipe(fd);
    while (1)
    {
        str = readline("> ");
        if (!str)
            break;
        if (ft_strncmp(str, node->cmd[1], ft_strlen(str)) == 0)
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
    write(fd[1], here_str, ft_strlen(here_str));
    /* // test code
    printf("--- heredoc  \n%s", here_str);
    // */
    free(here_str);
    close(fd[1]);
    return (fd[0]);
}

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
                if (tmp->type == INPUT)
                {
                    tmp->infile = open(tmp->cmd[1], O_RDONLY);
                    if (tmp->infile == -1)
                    {
                        printf("minishell: %s: No such file or directory\n", tmp->cmd[1]);
                        free_node_all(node); // free token??
                        return (0);
                    }
                }   
                else
                    tmp->infile = get_heredoc_fd(tmp);
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
                target->infile = prev->infile;
                target->outfile = prev->outfile;
            }
            prev = tmp;
            tmp = tmp->nxt; // target 다음
            while (tmp && (tmp->type ==  INPUT || tmp->type ==  HEREDOC))
            {
                // if (target->infile != -1 && target->infile != 0)
                //     close(target->infile);
                if (tmp->type == INPUT)
                {
                    target->infile = open(tmp->cmd[1], O_RDONLY);
                    printf("[%s, %d]\n", tmp->cmd[1], target->infile);
                    if (target->infile == -1)
                    {
                        printf("minishell: %s: No such file or directory\n", tmp->cmd[1]);
                        free_node_all(node); // free token??
                        return (0);
                    }
                }
                else
                    target->infile = get_heredoc_fd(tmp);
                prev = tmp;
                tmp = tmp->nxt;
            } // INPUT 다음
            while (tmp && (tmp->type == TRUNC || tmp->type == APPEND))
            {
                // if (target->outfile != -1 && target->outfile != 1)
                //     close(target->outfile);
                if (tmp->type == APPEND)
                    target->outfile = open(tmp->cmd[1], O_CREAT | O_WRONLY | O_APPEND, 0666);
                else if (tmp->type == TRUNC)
                    target->outfile = open(tmp->cmd[1], O_CREAT | O_WRONLY | O_TRUNC, 0666);
                prev = tmp;
                tmp = tmp->nxt;
            }
        }
        if (flag)
        {
            prev = tmp;
            tmp = tmp->nxt;
        }
    }
    return (node);
}

t_node *exec_unit(t_token *token)
{
    t_node *head;
    t_token *tmp;
    t_token *start;
    int i = 0;

    head = NULL;
    tmp = token;
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
            head = add_node(head, start, i, CMD);
        }
        else if (tmp->type == PIPE || tmp->type == END)
        {
            head = add_node(head, start, 1, tmp->type);
            tmp = tmp->nxt;
        }
        else if (tmp->type == TRUNC || tmp->type == APPEND || tmp->type == INPUT || tmp->type == HEREDOC)
        {
            if (!tmp->nxt)
            {
                printf("minishell: syntax error near unexpected token `newline'\n");
                return (0);
            }
            head = add_node(head, start, 2, tmp->type);
            tmp = tmp->nxt;
            tmp = tmp->nxt;
        }
        if (!tmp)
            break;
    }
    head = get_fd(head);
    return (head);
}