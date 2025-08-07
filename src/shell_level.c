/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_level.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daflynn <daflynn@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 09:35:30 by daflynn           #+#    #+#             */
/*   Updated: 2025/08/07 09:35:31 by daflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_increment_shlvl(t_va *node)
{
	int	level;

	level = ft_atoi(node->value) + 1;
	if (level < 1)
		level = 1;
	free(node->value);
	node->value = ft_itoa(level);
	if (!node->value)
		return (0);
	return (1);
}

int	ft_create_shlvl(t_va **env_list)
{
	t_va	*new_node;

	new_node = malloc(sizeof(t_va));
	if (!new_node)
		return (0);
	new_node->name = ft_strdup("SHLVL");
	new_node->value = ft_itoa(1);
	if (!new_node->name || !new_node->value)
	{
		free(new_node->name);
		free(new_node->value);
		free(new_node);
		return (0);
	}
	new_node->next = *env_list;
	*env_list = new_node;
	return (1);
}

void	ft_update_shlvl(t_va **env_list)
{
	t_va	*cur;

	cur = *env_list;
	while (cur)
	{
		if (ft_strcmp(cur->name, "SHLVL") == 0)
		{
			if (!ft_increment_shlvl(cur))
				perror("minishell: malloc error");
			return ;
		}
		cur = cur->next;
	}
	if (!ft_create_shlvl(env_list))
		perror("minishell: malloc error");
}
