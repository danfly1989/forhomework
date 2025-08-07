/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   control.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daflynn <daflynn@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 17:26:58 by daflynn           #+#    #+#             */
/*   Updated: 2025/08/05 17:27:09 by daflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_dat	ft_duplicate_input_args(int argc, char **argv, char **env)
{
	t_dat	data;

	(void)argc;
	data.av = NULL;
	data.ev = NULL;
	data.lo = NULL;
	data.ln = NULL;
	data.xln = NULL;
	data.tmp1 = NULL;
	data.tmp2 = NULL;
	data.i = 0;
	data.j = 0;
	data.k = 0;
	data.tot = 0;
	data.avs = NULL;
	data.evs = NULL;
	data.av = create_lst_frm_arr(argv + 1, NULL, 0, ft_create_node);
	data.ev = create_lst_frm_arr(env, NULL, 0, ft_create_var_node);
	ft_update_shlvl(&data.ev);
	return (data);
}

void	ft_cleanup_data(t_dat *data)
{
	if (data->ev != NULL)
		ft_free_list(data->ev);
	if (data->av != NULL)
		ft_free_list(data->av);
	if (data->lo != NULL)
		ft_free_list(data->lo);
	if (data->ln != NULL)
		ft_free_string_array(data->ln);
	if (data->xln != NULL)
		ft_free_string_array(data->xln);
	if (data->tmp1 != NULL)
		free(data->tmp1);
	if (data->tmp2 != NULL)
		free(data->tmp2);
}

void	ft_exit(t_dat *data, size_t k)
{
	int	status;

	if (data->xln[k + 1] && data->xln[k + 2])
	{
		write(2, "minishell: exit: too many arguments\n", 36);
		return ;
	}
	rl_clear_history();
	if (data->xln[k + 1] == NULL)
		ft_cleanup_exit(data, 0);
	if (ft_is_number(data->xln[k + 1]) == 0)
	{
		ft_exit_numeric_error(data->xln[k + 1]);
		ft_cleanup_exit(data, 255);
	}
	status = ft_atoi(data->xln[k + 1]);
	ft_cleanup_exit(data, status % 256);
}

void	ft_exit_numeric_error(char *arg)
{
	write(2, "minishell: exit: ", 18);
	write(2, arg, ft_strlen(arg));
	write(2, ": numeric argument required\n", 29);
}

void	ft_handle_builtin(t_dat *data, char *line, size_t k)
{
	(void)line;
	if (data == NULL || data->xln == NULL)
		return ;
	if (ft_strcmp(data->xln[k], "pwd") == 0)
		ft_pwd();
	else if (ft_strcmp(data->xln[k], "cd") == 0)
		ft_change_directory(data, k);
	else if (ft_strcmp(data->xln[k], "echo") == 0)
		ft_echo(data, k);
	else if (ft_strcmp(data->xln[k], "exit") == 0)
		ft_exit(data, k);
	else if (ft_strcmp(data->xln[k], "env") == 0)
		ft_env(data);
	else if (ft_strcmp(data->xln[k], "unset") == 0)
		ft_unset_multi_var(data, k);
	else if (ft_strcmp(data->xln[k], "export") == 0)
		ft_export_multi_var(data, k);
	else
		ft_external_functions(data, line);
}
