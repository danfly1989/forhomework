/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daflynn <daflynn@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 18:40:22 by daflynn           #+#    #+#             */
/*   Updated: 2025/08/05 18:40:40 by daflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_echo(t_dat *d, size_t k)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	while (d->xln[k + i] != NULL && ft_strncmp(d->xln[k + i], "-n", 2) == 0)
	{
		newline = 0;
		i++;
	}
	while (d->xln[k + i] != NULL)
	{
		printf("%s", d->xln[k + i]);
		i++;
		if (d->xln[k + i] != NULL)
			printf(" ");
	}
	if (newline)
		printf("\n");
}
