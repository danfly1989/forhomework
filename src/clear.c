/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clear.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daflynn <daflynn@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 09:20:32 by daflynn           #+#    #+#             */
/*   Updated: 2025/08/07 09:20:41 by daflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	***ft_clean_cmd(char ***cmd)
{
	int	i;

	if (!cmd)
		return (NULL);
	i = 0;
	while (cmd[i])
	{
		ft_free_string_array(cmd[i]);
		i++;
	}
	free(cmd);
	return (NULL);
}

void	ft_free_fd(int **fd)
{
	int	i;

	if (!fd)
		return ;
	i = 0;
	while (fd[i])
	{
		free(fd[i]);
		i++;
	}
	free(fd);
}

void	ft_cleanup_exit(t_dat *data, int flag)
{
	ft_cleanup_data(data);
	rl_clear_history();
	exit(flag);
}

void	ft_unset_multi_var(t_dat *d, size_t k)
{
	int	i;

	i = 1;
	while (d->xln[k + i] != NULL)
	{
		d->ev = ft_remove_variable_node(d->xln[k + i], d->ev, NULL);
		d->lo = ft_remove_variable_node(d->xln[k + i], d->lo, NULL);
		i++;
	}
}
