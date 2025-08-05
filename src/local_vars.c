/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   local_vars.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daflynn <daflynn@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 18:19:05 by daflynn           #+#    #+#             */
/*   Updated: 2025/08/05 18:19:13 by daflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_add_local_var(t_dat *data, char *str)
{
	t_va	*new_node;
	t_va	*last;

	new_node = ft_create_var_node(str);
	if (!new_node)
		return (0);
	if (!data->lo)
	{
		data->lo = new_node;
		return (1);
	}
	last = data->lo;
	while (last->next)
		last = last->next;
	last->next = new_node;
	return (1);
}

void	ft_update_local_variables(t_dat *d)
{
	int		i;
	char	*name;
	t_va	*node;

	if (!d || !d->xln)
		return ;
	i = 0;
	while (d->xln[i])
	{
		name = ft_extract_var_name(d->xln[i]);
		if (!name)
			return ;
		node = ft_find_var(d->lo, name);
		if (node)
			ft_update_var_value(node, ft_strchr(d->xln[i], '=') + 1);
		else
			ft_add_local_var(d, d->xln[i]);
		free(name);
		i++;
	}
}

int	ft_all_valid_lvar(t_dat *data, char **arr)
{
	int	i;

	if (!arr)
		return (0);
	ft_reset_iterators(data);
	i = 0;
	while (arr[i])
		i++;
	data->tot = i;
	i = 0;
	while (data->xln[i])
	{
		if (!ft_valid_var(arr[i]))
		{
			data->k = i;
			return (0);
		}
		i++;
	}
	return (1);
}
