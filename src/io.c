/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daflynn <daflynn@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 09:30:02 by daflynn           #+#    #+#             */
/*   Updated: 2025/08/07 09:30:08 by daflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	**init_fd_array(int tot)
{
	int	**fd;
	int	i;

	fd = malloc(sizeof(int *) * tot);
	if (!fd)
		return (NULL);
	i = 0;
	while (i < tot - 1)
	{
		fd[i] = malloc(sizeof(int) * 2);
		if (!fd[i])
		{
			while (--i >= 0)
				free(fd[i]);
			free(fd);
			return (NULL);
		}
		i++;
	}
	fd[tot - 1] = NULL;
	return (fd);
}

int	ft_handle_heredoc(char *delim, char *line)
{
	size_t	len;
	int		pipefd[2];

	len = 0;
	signal(SIGINT, SIG_DFL);
	if (pipe(pipefd) == -1)
		return (perror("pipe heredoc"), 0);
	while (1)
	{
		write(1, "> ", 2);
		if (getline(&line, &len, stdin) == -1)
			break ;
		if (ft_strncmp(line, delim, strlen(delim)) == 0
			&& line[strlen(delim)] == '\n')
			break ;
		write(pipefd[1], line, strlen(line));
	}
	free(line);
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	return (1);
}

void	ft_setup_io(int **fd, size_t i, size_t total)
{
	if (i > 0)
		dup2(fd[i - 1][0], STDIN_FILENO);
	if (i < total - 1)
		dup2(fd[i][1], STDOUT_FILENO);
}
