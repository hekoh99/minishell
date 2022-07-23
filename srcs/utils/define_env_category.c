/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hako <hako@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 22:25:48 by hako              #+#    #+#             */
/*   Updated: 2022/07/23 15:25:36 by hako             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int is_general_env_var(char *target, int squote)
{
    if (*target == '$' && (*(target + 1) == '?' || ft_isalpha(*(target + 1)) || ft_isdigit(*(target + 1))) && squote == 0)
        return (1);
    return (0);
}

int is_home_env_var(char *target, int squote)
{
    if (target[0] == '~' && (ft_strlen(target) == 1 || target[1] == '/') && squote == 0)
        return (1);
    return (0);
}
