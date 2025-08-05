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
	char	*cmd_path;

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

void	ft_ex_single_cmd(t_dat *d, char *cmd_path)
{
	pid_t	pid;
	t_rdr	r;

	ft_parse_redirection(d->xln, &r);
	if (!ft_apply_sing_redirections(&r, d->xln))
		exit(1);
	pid = fork();
	if (pid == 0)
	{
		ft_set_default_signals();
		cmd_path = ft_get_cmd_path(d, d->xln[0], 0);
		if (!cmd_path)
			ft_cmd_not_found(d->xln[0]);
		execve(cmd_path, d->xln, d->evs);
		exit(1);
	}
	else if (pid > 0)
		waitpid(pid, NULL, 0);
	else
		perror("fork");
}

int	ft_parse_cmd_helper(t_dat *d, char ***cmd, int *idx, int *st_i)
{
	int	i;

	i = st_i[1];
	if (!ft_validate_segment(d->xln, st_i[0], i))
		return (0);
	cmd[*idx] = ft_extract_tokens(d, st_i[0], i);
	if (!cmd[*idx])
		return (0);
	(*idx)++;
	st_i[0] = i + 1;
	return (1);
}

char	***ft_parse_cmd(t_dat *d, int st, int i, int idx)
{
	char ***cmd;
	int st_i[2];

	d->k = ft_count_pipes(d->xln) + 1;
	cmd = malloc((d->k + 1) * sizeof(char **));
	if (!cmd)
		return (NULL);
	st_i[0] = st;
	while (1)
	{
		st_i[1] = i;
		if (!d->xln[i] || !ft_strcmp(d->xln[i], "|"))
		{
			if (!ft_parse_cmd_helper(d, cmd, &idx, st_i))
				return (ft_clean_cmd(cmd));
			if (!d->xln[i])
				break ;
		}
		i++;
	}
	cmd[idx] = NULL;
	d->tot = idx;
	return (cmd);
}