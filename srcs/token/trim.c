/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trim.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubchoi <yubchoi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 22:26:40 by hako              #+#    #+#             */
/*   Updated: 2022/07/23 17:02:07 by hako             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*do_trim_space(t_token *head,
		char *line, int *start, int i)
{
	if (i - (*start) > 0)
		head = add_token(head, ft_substr(line, (*start), i - (*start)));
	(*start) = i + 1;
	return (head);
}

t_token	*trim_space(char *line)
{
	t_token		*head;
	int			squote;
	int			dquote;
	int			start;
	int			i;

	start = 0;
	i = 0;
	dquote = 0;
	squote = 0;
	head = NULL;
	while (line[i] != '\0')
	{
		check_quote(line[i], &squote, &dquote);
		if ((line[i] == ' ') && (squote == 0 && dquote == 0))
			head = do_trim_space(head, line, &start, i);
		i++;
		if ((line[i] == '\0') && (squote == 0 && dquote == 0))
			head = do_trim_space(head, line, &start, i);
	}
	if (squote == 1 || dquote == 1)
		return (open_quote_err(head));
	return (head);
}

char	*inside_quote(char *value, int start, int *mid)
{
	char	*inside;
	char	*head;
	char	*tail;
	char	*str;

	str = ft_substr(value, start + 1, *mid - start - 1);
	head = ft_substr(value, 0, start);
	tail = ft_substr(value, *mid + 1, ft_strlen(value));
	free(value);
	inside = ft_strjoin(head, str);
	*mid = ft_strlen(inside) - 1;
	inside = ft_strjoin_free_s1(inside, tail);
	free(str);
	free(head);
	free(tail);
	return (inside);
}

void	do_trim_quote(t_token *tmp)
{
	int	squote;
	int	dquote;
	int	i;
	int	start;

	i = 0;
	squote = 0;
	dquote = 0;
	start = -1;
	while (tmp->value && tmp->value[i] != '\0')
	{
		check_quote(tmp->value[i], &squote, &dquote);
		if ((tmp->value[i] == '\"' || tmp->value[i] == '\'')
			&& squote + dquote > 0 && start == -1)
			start = i;
		else if ((tmp->value[i] == '\"' || tmp->value[i] == '\'')
			&& squote + dquote == 0 && start != -1)
		{
			tmp->value = inside_quote(tmp->value, start, &i);
			start = -1;
		}
		i++;
	}
}

t_token	*trim_quote(t_token *token)
{
	t_token	*tmp;

	tmp = token;
	while (tmp)
	{
		do_trim_quote(tmp);
		tmp = tmp->nxt;
	}
	return (token);
}
