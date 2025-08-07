/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daflynn <daflynn@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 20:22:36 by daflynn           #+#    #+#             */
/*   Updated: 2025/08/05 20:22:47 by daflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_check_redir(char **tokens, int i)
{
	if (!tokens[i + 1] || ft_is_redirection(tokens[i + 1])
		|| !ft_strcmp(tokens[i + 1], "|"))
		return (ft_syntax_error(tokens[i + 1]));
	return (1);
}

int	ft_validate_syntax(char **tokens)
{
	int	i;

	if (!tokens || !tokens[0])
		return (0);
	i = 0;
	while (tokens[i])
	{
		if (ft_is_redirection(tokens[i]))
		{
			if (!ft_check_redir(tokens, i))
				return (0);
		}
		else if (!ft_strcmp(tokens[i], "|"))
		{
			if (!ft_check_pipe(tokens, i))
				return (0);
		}
		i++;
	}
	return (1);
}

int	ft_syntax_error(char *token)
{
	char	*mes1;
	char	*mes2;

	mes1 = "minishell: syntax error near unexpected token `";
	mes2 = "minishell: syntax error near unexpected token `newline'\n";
	if (token)
	{
		write(2, mes1, ft_strlen(mes1));
		write(2, token, ft_strlen(token));
		write(2, "'\n", 2);
	}
	else
		write(2, mes2, ft_strlen(mes2));
	return (0);
}

int	ft_syntax_error_msg(char *token)
{
	char	*prefix;
	char	*newline;

	prefix = "minishell: syntax error near unexpected token `";
	newline = "'\n";
	if (token)
	{
		write(2, prefix, ft_strlen(prefix));
		write(2, token, ft_strlen(token));
		write(2, newline, 2);
	}
	else
	{
		write(2, prefix, ft_strlen(prefix));
		write(2, "newline", 7);
		write(2, newline, 2);
	}
	return (0);
}

int	ft_validate_segment(char **tokens, int start, int end)
{
	int	i;

	if (!tokens || start >= end)
		return (0);
	i = start;
	while (i < end)
	{
		if (ft_is_redirection(tokens[i]))
		{
			if (i + 1 >= end || ft_is_redirection(tokens[i + 1])
				|| !ft_strcmp(tokens[i + 1], "|"))
			{
				return (ft_syntax_error_msg(tokens[i + 1]));
			}
		}
		i++;
	}
	return (1);
}
