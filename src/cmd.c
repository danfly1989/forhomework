/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daflynn <daflynn@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 19:41:24 by daflynn           #+#    #+#             */
/*   Updated: 2025/08/05 19:41:34 by daflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_get_cmd_path(t_dat *d, const char *cmd, int i)
{
	char	*full_path;

	if (!cmd || cmd[0] == '\0')
		return (NULL);
	d->tmp1 = ft_get_val_from_list(d->ev, "PATH");
	if (!d->tmp1)
		return (NULL);
	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	d->avs = ft_split(d->tmp1, ':');
	while (d->avs && d->avs[i])
	{
		full_path = ft_join_path(d->avs[i], (char *)cmd);
		if (access(full_path, X_OK) == 0)
		{
			ft_free_string_array(d->avs);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	ft_free_string_array(d->avs);
	return (NULL);
}

void	ft_exec_command(t_dat *d, char **cmd)
{
	char *cmd_path;

	cmd_path = ft_get_cmd_path(d, cmd[0], 0);
	if (!cmd_path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd[0], 2);
		ft_putendl_fd(": command not found", 2);
		exit(127);
	}
	execve(cmd_path, cmd, d->evs);
	perror("execve");
	exit(1);
}