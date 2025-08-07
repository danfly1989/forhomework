/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daflynn <daflynn@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 18:07:46 by daflynn           #+#    #+#             */
/*   Updated: 2025/08/05 18:07:53 by daflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_expand_loop(char *token, t_dat *data, char **res, size_t *i)
{
	char	*key;
	char	*val;
	char	*tmp;

	key = ft_extract_var_key(token, i);
	if (!key)
		return ;
	val = NULL;
	if (data->lo != NULL)
		val = ft_get_var_value(data->lo, key);
	if (val == NULL)
		val = ft_get_var_value(data->ev, key);
	if (val != NULL)
	{
		tmp = *res;
		*res = ft_strjoin(*res, val);
		free(tmp);
	}
	free(key);
}

char	*ft_expand_token(char *token, t_dat *data, int qt, size_t i)
{
	char	*res;
	char	*tmp;

	if (qt == 1)
		return (ft_strdup(token));
	res = ft_calloc(1, sizeof(char));
	while (token[i])
	{
		if (token[i] == '$' && token[i + 1] && (ft_isalpha(token[i + 1])
				|| token[i + 1] == '_' || token[i + 1] == '?'))
		{
			i++;
			ft_expand_loop(token, data, &res, &i);
		}
		else
		{
			tmp = res;
			res = ft_strjoin_char(res, token[i++]);
			free(tmp);
		}
	}
	return (res);
}

char	**ft_expand_tokens(t_dat *d, char **tokens, int *qtypes, int i)
{
	char	**expanded;

	while (tokens[i])
		i++;
	expanded = malloc(sizeof(char *) * (i + 1));
	if (!expanded)
		return (NULL);
	i = 0;
	while (tokens[i])
	{
		expanded[i] = ft_expand_token(tokens[i], d, qtypes[i], 0);
		if (!expanded[i])
		{
			while (--i >= 0)
				free(expanded[i]);
			free(expanded);
			return (NULL);
		}
		i++;
	}
	expanded[i] = NULL;
	return (expanded);
}

void	ft_check_var_assign_and_expand_line(t_dat *data, char *line)
{
	int	*quote_types;

	if (!data || !line)
		return ;
	data->ln = ft_tokenize_line(data, line, &quote_types);
	if (!data->ln)
		return ;
	data->xln = ft_expand_tokens(data, data->ln, quote_types, 0);
	if (!data->xln)
	{
		free(quote_types);
		return ;
	}
	ft_strip_quotes_from_xln(data);
	if (ft_all_valid_lvar(data, data->xln))
		ft_update_local_variables(data);
	else if (data->k)
		ft_handle_builtin(data, line, data->k);
	else
		ft_handle_builtin(data, line, 0);
	free(quote_types);
}
