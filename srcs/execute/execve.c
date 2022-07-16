/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubchoi <yubchoi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/16 12:14:26 by yubchoi           #+#    #+#             */
/*   Updated: 2022/07/16 12:40:34 by yubchoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char **find_path(t_env *envp)
{
    int i;
    char **path;

    i = -1;
    path = NULL;
    while (envp)
    {
        if (ft_strncmp(envp->key, "PATH", 4) == 0)
        {
            path = ft_split((envp->value), ':');
            return (path);
        }
        envp = envp->nxt;
    }
    free_matrix(path);
    error_exit("PATH not found", 127);
    return (NULL);
}

static char *find_cmd_path(char *cmd, char **paths)
{
    int i;
    char *cmd_path;
    char *tmp_cmd;

    i = -1;
    if (ft_strchr(cmd, '/'))
        return (cmd);
    while (paths && paths[++i])
    {
        tmp_cmd = ft_strjoin("/", cmd);
        cmd_path = ft_strjoin(paths[i], tmp_cmd);
        if (access(cmd_path, X_OK) == 0)
        {
            free(tmp_cmd);
            return (cmd_path);
        }
        free(tmp_cmd);
        free(cmd_path);
    }
    return (NULL);
}

int count_envp(t_env *envp)
{
    int i;

    i = 0;
    while (envp)
    {
        ++i;
        envp = envp->nxt;
    }
    return (i);
}

char **init_envp_arr(t_env *envp)
{
    int i;
    char **envp_arr;

    i = -1;
    envp_arr = (char **)malloc(sizeof(char *) * (count_envp(envp) + 1));
    while (envp)
    {
        envp_arr[++i] = ft_strjoin(envp->key, "=");
        envp_arr[i] = ft_strjoin(envp_arr[i], envp->value);
        envp = envp->nxt;
    }
    envp_arr[++i] = NULL;
    return (envp_arr);
}

void init_arg(t_node *node, t_exec_arg *arg)
{
    char **paths;

    paths = find_path(node->envp);
    arg->argv = node->cmd;
    arg->file = find_cmd_path(arg->argv[0], paths);
    arg->envp = init_envp_arr(node->envp);
    free_matrix(paths);
}

void do_execve(t_node *node)
{
    t_exec_arg arg;

    init_arg(node, &arg);
    ft_execve(arg.file, arg.argv, arg.envp);
    free_matrix(arg.envp);
}