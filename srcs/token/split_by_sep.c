/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_by_sep.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubchoi <yubchoi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 22:26:08 by hako              #+#    #+#             */
/*   Updated: 2022/07/23 15:53:15 by hako             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_token	*join_list_center(t_token *pos,
		int sep_size, int *index, int size)
{
	t_token	*separated;
	char	*tmp;

	separated = malloc(sizeof(t_token) * sep_size);
	separated->value = ft_substr(pos->value, *index, sep_size);
	separated->nxt = pos->nxt;
	pos->nxt->prev = separated;
	pos->nxt = separated;
	separated->prev = pos;
	pos = pos->nxt;
	separated = malloc(sizeof(t_token));
	separated->value = ft_substr(pos->prev->value, *index + sep_size, size);
	separated->nxt = pos->nxt;
	pos->nxt->prev = separated;
	pos->nxt = separated;
	separated->prev = pos;
	tmp = pos->prev->value;
	pos->prev->value = ft_substr(tmp, 0, *index);
	free(tmp);
	if (sep_size == 2)
		(*index)++;
	return (pos);
}

static t_token	*join_list_back(t_token *pos,
		int sep_size, int *index, int size)
{
	t_token	*separated;
	char	*tmp;

	separated = malloc(sizeof(t_token));
	separated->value = ft_substr(pos->value, *index, sep_size);
	pos->nxt = separated;
	separated->prev = pos;
	separated->nxt = NULL;
	pos = pos->nxt;
	separated = malloc(sizeof(t_token));
	separated->value = ft_substr(pos->prev->value, *index + sep_size, size);
	pos->nxt = separated;
	separated->prev = pos;
	separated->nxt = NULL;
	tmp = pos->prev->value;
	pos->prev->value = ft_substr(tmp, 0, *index);
	free(tmp);
	if (sep_size == 2)
		(*index)++;
	return (pos);
}

static t_token	*do_split_by_seps(t_token *pos,
		int sep_size, int *index, int size)
{
	if (pos->nxt != NULL)
	{
		pos = join_list_center(pos, sep_size, index, size);
	}
	else
	{
		pos = join_list_back(pos, sep_size, index, size);
	}
	return (pos);
}

static t_token	*split_target_token(t_token *token,
		t_token *tmp, int *squote, int *dquote)
{
	int		i;
	char	*sep;
	int		sep_size;
	int		size;

	i = 0;
	sep = ";|><";
	size = ft_strlen(tmp->value);
	while (tmp->value[i] != '\0')
	{
		check_quote(tmp->value[i], squote, dquote);
		if (ft_strchr(sep, tmp->value[i]) != 0 && *squote == 0 && *dquote == 0)
		{
			sep_size = check_duple_sep(tmp->value, i);
			if (sep_size == 0)
			{
				free_token_all(token);
				return (NULL);
			}
			tmp = do_split_by_seps(tmp, sep_size, &i, size);
		}
		i++;
	}
	return (tmp);
}

t_token	*split_by_sep(t_token *token)
{
	t_token	*tmp;
	int		squote;
	int		dquote;

	tmp = token;
	squote = 0;
	dquote = 0;
	while (tmp)
	{
		tmp = split_target_token(token, tmp, &squote, &dquote);
		if (!tmp)
			return (NULL);
		tmp = tmp->nxt;
	}
	token = ft_dellist(token, "");
	return (token);
}
