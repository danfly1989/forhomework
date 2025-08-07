/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daflynn <daflynn@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 17:18:19 by daflynn           #+#    #+#             */
/*   Updated: 2025/08/07 16:48:07 by daflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_strip_surrounding_quotes(char *s)
{
	size_t	len;
	size_t	j;

	len = ft_strlen(s);
	if (len >= 2 && ((s[0] == '"' && s[len - 1] != '"') || (s[0] == '\''
				&& s[len - 1] != '\'')))
		return ;
	if (len >= 2 && ((s[0] == '"' && s[len - 1] == '"') || (s[0] == '\''
				&& s[len - 1] == '\'')))
	{
		j = 1;
		while (j < len - 1)
		{
			s[j - 1] = s[j];
			j++;
		}
		s[j - 1] = '\0';
	}
}

void	ft_strip_quotes_after_equal(char *s)
{
	char	*eq;
	char	quote;
	size_t	j;
	size_t	len;

	eq = ft_strchr(s, '=');
	len = ft_strlen(s);
	if (eq && ((eq[1] == '"' && s[len - 1] != '"') || (eq[1] == '\'' && s[len
					- 1] != '\'')))
		return ;
	if (eq && ((eq[1] == '"' && s[len - 1] == '"') || (eq[1] == '\'' && s[len
					- 1] == '\'')))
	{
		quote = eq[1];
		j = 0;
		while (eq[2 + j] && eq[2 + j] != quote)
		{
			eq[1 + j] = eq[2 + j];
			j++;
		}
		eq[1 + j] = '\0';
	}
}

void	ft_strip_quotes_from_xln(t_dat *d)
{
	size_t	i;

	i = 0;
	while (d->xln[i])
	{
		ft_strip_surrounding_quotes(d->xln[i]);
		ft_strip_quotes_after_equal(d->xln[i]);
		i++;
	}
}

void	ft_detect_quote_type(char *token, int *quote_type)
{
	char	*eq;
	char	quote;

	if (token[0] == '\'')
		*quote_type = 1;
	else if (token[0] == '"')
		*quote_type = 2;
	else
	{
		eq = ft_strchr(token, '=');
		if (eq && (*(eq + 1) == '\'' || *(eq + 1) == '"'))
		{
			quote = *(eq + 1);
			if (quote == '\'')
				*quote_type = 1;
			else
				*quote_type = 2;
		}
	}
}
