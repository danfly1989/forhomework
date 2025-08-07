/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daflynn <daflynn@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 18:00:58 by daflynn           #+#    #+#             */
/*   Updated: 2025/08/05 18:01:07 by daflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_count_tokens(char *str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		while (str[i] == ' ')
			i++;
		if (!str[i])
			break ;
		count++;
		if (str[i] == '\'' || str[i] == '"')
			i = ft_skip_quote(str, i);
		else
			i = ft_skip_token(str, i);
	}
	return (count);
}

int	ft_get_token_end(char *str, int i)
{
	if (str[i] == '\'' || str[i] == '"')
		return (ft_skip_quote(str, i));
	return (ft_skip_token(str, i));
}

char	**ft_extract_tokens(t_dat *data, int start, int end)
{
	char	**tokens;
	int		i;

	tokens = malloc((end - start + 1) * sizeof(char *));
	if (!tokens)
		return (NULL);
	i = 0;
	while (start < end)
	{
		tokens[i] = ft_strdup(data->xln[start]);
		if (!tokens[i])
		{
			ft_free_string_array(tokens);
			return (NULL);
		}
		start++;
		i++;
	}
	tokens[i] = NULL;
	return (tokens);
}
