/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piping.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daflynn <daflynn@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 19:47:12 by daflynn           #+#    #+#             */
/*   Updated: 2025/08/05 19:47:20 by daflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_check_pipe(char **tokens, int i)
{
	if (i == 0 || !tokens[i + 1] || ft_is_redirection(tokens[i + 1]))
		return (ft_syntax_error(tokens[i + 1]));
	return (1);
}

void	ft_execute_pipeline(t_dat *d, char ***cmd)
{
	int	**fd;

	fd = init_fd_array(d->tot);
	if (!fd || !ft_create_pipes(fd, d->tot))
	{
		if (fd)
			ft_free_fd(fd);
		return ;
	}
	ft_fork_children(d, cmd, fd);
	ft_close_pipes(fd, d->tot);
	ft_wait_children(d->tot);
	ft_free_fd(fd);
}

int	ft_create_pipes(int **fd, int tot)
{
	int i;

	i = 0;
	while (i < tot - 1)
	{
		if (pipe(fd[i]) == -1)
		{
			perror("pipe");
			while (i-- > 0)
				free(fd[i]);
			free(fd);
			return (0);
		}
		i++;
	}
	return (1);
}