/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_management.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daflynn <daflynn@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 18:42:57 by daflynn           #+#    #+#             */
/*   Updated: 2025/08/05 18:43:20 by daflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_va	*ft_merge_sorted_lists(t_va *a, t_va *b)
{
	t_va	*result;

	if (a == NULL)
		return (b);
	else if (b == NULL)
		return (a);
	if (ft_strcmp(a->name, b->name) <= 0)
	{
		result = a;
		result->next = ft_merge_sorted_lists(a->next, b);
	}
	else
	{
		result = b;
		result->next = ft_merge_sorted_lists(a, b->next);
	}
	return (result);
}

void	ft_split_list(t_va *source, t_va **front, t_va **back)
{
	t_va	*fast;
	t_va	*slow;

	slow = source;
	fast = source->next;
	while (fast != NULL)
	{
		fast = fast->next;
		if (fast != NULL)
		{
			slow = slow->next;
			fast = fast->next;
		}
	}
	*front = source;
	*back = slow->next;
	slow->next = NULL;
}

void	ft_sort_list_by_name(t_va **head_ref)
{
	t_va	*head;
	t_va	*a;
	t_va	*b;

	if (*head_ref == NULL || (*head_ref)->next == NULL)
		return ;
	head = *head_ref;
	ft_split_list(head, &a, &b);
	ft_sort_list_by_name(&a);
	ft_sort_list_by_name(&b);
	*head_ref = ft_merge_sorted_lists(a, b);
}

t_va	*ft_duplicate_list(const t_va *head)
{
	const t_va	*cur;
	t_va		*new_head;
	t_va		*new_tail;

	cur = head;
	new_head = NULL;
	new_tail = NULL;
	while (cur != NULL)
	{
		if (!ft_append_dup_node(cur, &new_head, &new_tail))
			return (NULL);
		cur = cur->next;
	}
	return (new_head);
}

int	ft_count_list(t_va *head)
{
	t_va	*cur;
	int		count;

	cur = head;
	count = 0;
	while (cur)
	{
		if (cur->name)
			count++;
		cur = cur->next;
	}
	return (count);
}
