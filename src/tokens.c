/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daflynn <daflynn@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 17:02:54 by daflynn           #+#    #+#             */
/*   Updated: 2025/08/05 17:02:58 by daflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_skip_quote(char *str, int i)
{
	char	quote;

	quote = str[i++];
	while (str[i] && str[i] != quote)
		i++;
	if (str[i])
		i++;
	return (i);
}

int	ft_skip_token(char *str, int i)
{
	while (str[i] && str[i] != ' ')
	{
		if (str[i] == '\'' || str[i] == '"')
			i = ft_skip_quote(str, i);
		else
			i++;
	}
	return (i);
}

char	*ft_extract_token(char *str, t_dat *d, int *quote_type)
{
	int		start;
	int		end;
	char	*token;

	start = d->i;
	*quote_type = 0;
	end = ft_get_token_end(str, d->i);
	d->i = end;
	token = ft_strndup(str + start, end - start);
	if (!token)
		return (NULL);
	ft_detect_quote_type(token, quote_type);
	return (token);
}

char	**ft_free_token_quote(char **tokens, int *quote_types)
{
	if (tokens)
		free(tokens);
	if (quote_types)
		free(quote_types);
	return (NULL);
}

char	**ft_tokenize_line(t_dat *d, char *str, int **quote_types_out)
{
	char	**tokens;
	int		*quote_types;

	ft_reset_iterators(d);
	d->k = ft_count_tokens(str);
	tokens = malloc(sizeof(char *) * (d->k + 1));
	quote_types = malloc(sizeof(int) * (d->k + 1));
	if (!tokens || !quote_types)
		return (ft_free_token_quote(tokens, quote_types));
	while (str[d->i])
	{
		while (str[d->i] == ' ')
			d->i++;
		if (!str[d->i])
			break ;
		tokens[d->j] = ft_extract_token(str, d, &quote_types[d->j]);
		d->j++;
	}
	tokens[d->j] = NULL;
	quote_types[d->j] = -1;
	*quote_types_out = quote_types;
	return (tokens);
}
