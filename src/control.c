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

void	ft_cleanup_exit(t_dat *data, int flag)
{
	ft_cleanup_data(data);
	rl_clear_history();
	exit(flag);
}

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