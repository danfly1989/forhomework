/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nodes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daflynn <daflynn@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 18:55:34 by daflynn           #+#    #+#             */
/*   Updated: 2025/08/05 18:55:41 by daflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_va	*ft_remove_variable_node(const char *key, t_va *head, t_va *prev)
{
	t_va	*cur;
	t_va	*next;

	cur = head;
	while (cur != NULL)
	{
		if (ft_strncmp(cur->name, key, ft_strlen(key)) == 0)
		{
			next = cur->next;
			free(cur->name);
			free(cur->value);
			free(cur);
			if (prev == NULL)
				head = next;
			else
				prev->next = next;
			break ;
		}
		prev = cur;
		cur = cur->next;
	}
	return (head);
}

t_va	*ft_duplicate_node(const t_va *node)
{
	t_va	*new;

	new = malloc(sizeof(t_va));
	if (new == NULL)
		return (NULL);
	new->name = ft_strdup(node->name);
	new->value = ft_strdup(node->value);
	new->next = NULL;
	if (new->name == NULL || new->value == NULL)
	{
		free(new->name);
		free(new->value);
		free(new);
		return (NULL);
	}
	return (new);
}

int	ft_append_dup_node(const t_va *cur, t_va **head, t_va **tail)
{
	t_va	*new_node;

	new_node = ft_duplicate_node(cur);
	if (new_node == NULL)
	{
		ft_free_list(*head);
		return (0);
	}
	if (*tail == NULL)
	{
		*head = new_node;
		*tail = new_node;
	}
	else
	{
		(*tail)->next = new_node;
		*tail = new_node;
	}
	return (1);
}
