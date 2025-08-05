/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagwu <dagwu@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 18:44:41 by dagwu             #+#    #+#             */
/*   Updated: 2025/08/02 16:51:26 by daflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char *argv[], char *env[])
{
	char	*line;
	t_dat	data;

	data = ft_duplicate_input_args(argc, argv, env);
	while (1)
	{
		ft_set_main_signals();
		line = readline("dandav>");
		if (line == NULL)
			break ;
		if (line && *line && !ft_strisspace(line))
			add_history(line);
		ft_check_var_assign_and_expand_line(&data, line);
		if (line)
			free(line);
		ft_free_string_array(data.ln);
		ft_free_string_array(data.xln);
		data.ln = NULL;
		data.xln = NULL;
	}
	ft_cleanup_data(&data);
	return (0);
}

void	ft_unset_multi_var(t_dat *d, size_t k)
{
	int	i;

	i = 1;
	while (d->xln[k + i] != NULL)
	{
		d->ev = ft_remove_variable_node(d->xln[k + i], d->ev, NULL);
		d->lo = ft_remove_variable_node(d->xln[k + i], d->lo, NULL);
		i++;
	}
}

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

int	ft_var_name_only(char *str)
{
	size_t	i;

	if (!str)
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
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

void	ft_print_sorted_env(t_va *head)
{
	t_va	*sorted;

	sorted = ft_duplicate_list(head);
	ft_sort_list_by_name(&sorted);
	ft_print_export(sorted);
	ft_free_list(sorted);
}

void	ft_export_error(char *arg, char *message)
{
	write(2, "export: '", 9);
	write(2, arg, ft_strlen(arg));
	write(2, "': ", 3);
	write(2, message, ft_strlen(message));
	write(2, "\n", 1);
}

void	ft_export_multi_var(t_dat *data, size_t k)
{
	char	*message;
	int		i;

	message = "not a valid identifier";
	if (data->xln[k + 1] == NULL)
	{
		ft_print_sorted_env(data->ev);
		return ;
	}
	i = 1;
	while (data->xln[k + i] != NULL)
	{
		if (ft_valid_var(data->xln[k + i]) == 1)
		{
			ft_export_type1(&data->ev, data->xln[k + i], NULL, NULL);
			ft_add_local_var(data, data->xln[k + i]);
		}
		else if (ft_var_name_only(data->xln[k + i]) == 1)
			ft_export_type2(data, data->xln[k + i]);
		else
			ft_export_error(data->xln[k + i], message);
		i++;
	}
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

void	ft_check_var_assign_and_expand_line(t_dat *data, char *line)
{
	int	*quote_types;

	if (!data || !line)
		return ;
	data->ln = ft_tokenize_line(data, line, &quote_types);
	if (!data->ln)
		return ;
	data->xln = ft_expand_tokens(data, data->ln, quote_types, 0);
	if (!data->xln)
	{
		free(quote_types);
		return ;
	}
	ft_strip_quotes_from_xln(data);
	if (ft_all_valid_lvar(data, data->xln))
		ft_update_local_variables(data);
	else if (data->k)
		ft_handle_builtin(data, line, data->k);
	else
		ft_handle_builtin(data, line, 0);
	free(quote_types);
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

void	ft_list_to_env_array(t_dat *data)
{
	int		i;
	int		count;
	t_va	*cur;

	i = 0;
	data->tmp1 = NULL;
	count = ft_count_list(data->ev);
	data->evs = malloc((count + 1) * sizeof(char *));
	if (!data->evs)
		return ;
	cur = data->ev;
	while (cur && i < count)
	{
		data->tmp1 = ft_strjoin(cur->name, "=");
		data->evs[i] = ft_strjoin(data->tmp1, cur->value);
		free(data->tmp1);
		data->tmp1 = NULL;
		cur = cur->next;
		i++;
	}
	data->evs[i] = NULL;
}

char	*ft_join_path(char *str1, char *cmd)
{
	char	*temp;
	char	*full_path;

	temp = ft_strjoin(str1, "/");
	full_path = ft_strjoin(temp, cmd);
	free(temp);
	temp = NULL;
	return (full_path);
}

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

int	ft_count_pipes(char **tokens)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (tokens[i])
	{
		if (ft_strcmp(tokens[i], "|") == 0)
			count++;
		i++;
	}
	return (count);
}

void	ft_cmd_not_found(char *cmd)
{
	char	*prefix;
	char	*suffix;

	prefix = "minishell: ";
	suffix = ": command not found\n";
	write(2, prefix, ft_strlen(prefix));
	write(2, cmd, ft_strlen(cmd));
	write(2, suffix, ft_strlen(suffix));
	exit(127);
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

void	ft_external_functions(t_dat *data, char *line)
{
	char	***cmd;
	int		n;

	(void)line;
	if (!data || !data->xln || !data->xln[0])
		return ;
	if (!ft_validate_syntax(data->xln))
		return ;
	ft_list_to_env_array(data);
	n = ft_count_pipes(data->xln);
	if (n > 0)
	{
		cmd = ft_parse_cmd(data, 0, 0, 0);
		if (!cmd)
			return ;
		ft_execute_pipeline(data, cmd);
		ft_clean_cmd(cmd);
	}
	else
		ft_ex_single_cmd(data, NULL);
	ft_free_string_array(data->evs);
}

char	**ft_extract_tokens(t_dat *data, int start, int end)
{
	char	**tokens;
	int		i;

	tokens = malloc((end - start + 1) * sizeof(char *));
	if (!tokens)
		return (NULL);
	i = 0;
	while (start < end)
	{
		tokens[i] = ft_strdup(data->xln[start]);
		if (!tokens[i])
		{
			ft_free_string_array(tokens);
			return (NULL);
		}
		start++;
		i++;
	}
	tokens[i] = NULL;
	return (tokens);
}

char	***ft_clean_cmd(char ***cmd)
{
	int	i;

	if (!cmd)
		return (NULL);
	i = 0;
	while (cmd[i])
	{
		ft_free_string_array(cmd[i]);
		i++;
	}
	free(cmd);
	return (NULL);
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
	char	***cmd;
	int		st_i[2];

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

void	ft_free_fd(int **fd)
{
	int	i;

	if (!fd)
		return ;
	i = 0;
	while (fd[i])
	{
		free(fd[i]);
		i++;
	}
	free(fd);
}

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

int	ft_create_pipes(int **fd, int tot)
{
	int	i;

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

void	ft_setup_io(int **fd, size_t i, size_t total)
{
	if (i > 0)
		dup2(fd[i - 1][0], STDIN_FILENO);
	if (i < total - 1)
		dup2(fd[i][1], STDOUT_FILENO);
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

void	ft_child_process(t_dat *d, char ***cmd, int **fd, size_t i)
{
	t_rdr	r;
	int		len;

	len = 0;
	while (cmd[i][len])
		len++;
	ft_memset(&r, 0, sizeof(r));
	if (!ft_validate_segment(cmd[i], 0, len))
		exit(1);
	ft_setup_io(fd, i, d->tot);
	ft_close_pipes(fd, d->tot);
	ft_parse_redirection(cmd[i], &r);
	if (!ft_apply_redirections(&r, &cmd[i]))
		exit(1);
	ft_exec_command(d, cmd[i]);
}

int	ft_syntax_error_msg(char *token)
{
	char	*prefix;
	char	*newline;

	prefix = "minishell: syntax error near unexpected token `";
	newline = "'\n";
	if (token)
	{
		write(2, prefix, ft_strlen(prefix));
		write(2, token, ft_strlen(token));
		write(2, newline, 2);
	}
	else
	{
		write(2, prefix, ft_strlen(prefix));
		write(2, "newline", 7);
		write(2, newline, 2);
	}
	return (0);
}

int	ft_validate_segment(char **tokens, int start, int end)
{
	int	i;

	if (!tokens || start >= end)
		return (0);
	i = start;
	while (i < end)
	{
		if (ft_is_redirection(tokens[i]))
		{
			if (i + 1 >= end || ft_is_redirection(tokens[i + 1])
				|| !ft_strcmp(tokens[i + 1], "|"))
			{
				return (ft_syntax_error_msg(tokens[i + 1]));
			}
		}
		i++;
	}
	return (1);
}

void	ft_fork_children(t_dat *d, char ***cmd, int **fd)
{
	pid_t	pid;
	size_t	i;

	i = 0;
	while (i < d->tot)
	{
		pid = fork();
		if (pid == 0)
		{
			ft_set_default_signals();
			ft_child_process(d, cmd, fd, i);
		}
		else if (pid < 0)
			perror("fork");
		i++;
	}
}

void	ft_close_pipes(int **fd, int tot)
{
	int	i;

	i = 0;
	while (i < tot - 1)
	{
		close(fd[i][0]);
		close(fd[i][1]);
		i++;
	}
}

void	ft_wait_children(int tot)
{
	int	i;

	i = 0;
	while (i < tot)
	{
		wait(NULL);
		i++;
	}
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

int	ft_parse_redirection(char **tokens, t_rdr *r)
{
	int	i;

	i = 0;
	ft_memset(r, 0, sizeof(*r));
	while (tokens[i])
	{
		if (!ft_strcmp(tokens[i], "<") && tokens[i + 1])
			r->in_file = ft_strdup(tokens[++i]);
		else if (!ft_strcmp(tokens[i], ">") && tokens[i + 1])
			r->out_file = ft_strdup(tokens[++i]);
		else if (!ft_strcmp(tokens[i], ">>") && tokens[i + 1])
			r->append_file = ft_strdup(tokens[++i]);
		else if (!ft_strcmp(tokens[i], "<<") && tokens[i + 1])
			r->heredoc_delim = ft_strdup(tokens[++i]);
		i++;
	}
	return (1);
}

int	ft_redir_in(char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
		return (perror(file), 0);
	if (dup2(fd, STDIN_FILENO) < 0)
		return (perror("dup2 in"), 0);
	close(fd);
	return (1);
}

int	ft_redir_out(char *file)
{
	int	fd;

	fd = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		return (perror(file), 0);
	if (dup2(fd, STDOUT_FILENO) < 0)
		return (perror("dup2 out"), 0);
	close(fd);
	return (1);
}

int	ft_redir_append(char *file)
{
	int	fd;

	fd = open(file, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (fd < 0)
		return (perror(file), 0);
	if (dup2(fd, STDOUT_FILENO) < 0)
		return (perror("dup2 append"), 0);
	close(fd);
	return (1);
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

int	ft_apply_sing_redirections(t_rdr *r, char **tok)
{
	if (r->in_file && !ft_redir_in(r->in_file))
		return (perror("minishell"), 0);
	if (r->out_file && !ft_redir_out(r->out_file))
		return (perror("minishell"), 0);
	if (r->append_file && !ft_redir_append(r->append_file))
		return (perror("minishell"), 0);
	if (r->heredoc_delim && !ft_handle_heredoc(r->heredoc_delim, NULL))
		return (perror("minishell"), 0);
	ft_free_redirection(r);
	return (ft_remove_sing_redirections(tok, 0, 0));
}

int	ft_apply_redirections(t_rdr *r, char ***cmd)
{
	if (r->in_file && !ft_redir_in(r->in_file))
		return (0);
	if (r->out_file && !ft_redir_out(r->out_file))
		return (0);
	if (r->append_file && !ft_redir_append(r->append_file))
		return (0);
	if (r->heredoc_delim && !ft_handle_heredoc(r->heredoc_delim, NULL))
		return (0);
	ft_free_redirection(r);
	return (ft_remove_redirections(cmd, 0, 0));
}

int	ft_remove_redirections(char ***tokens_ptr, int i, int j)
{
	char	**tokens;

	if (!tokens_ptr || !*tokens_ptr)
		return (0);
	tokens = *tokens_ptr;
	while (tokens[i])
	{
		if (ft_is_redirection(tokens[i]) && tokens[i + 1])
		{
			free(tokens[i]);
			free(tokens[i + 1]);
			j = i - 1;
			while (tokens[++j + 2])
				tokens[j] = tokens[j + 2];
			tokens[j] = NULL;
			tokens[j + 1] = NULL;
			continue ;
		}
		i++;
	}
	*tokens_ptr = tokens;
	return (1);
}

int	ft_remove_sing_redirections(char **t, int i, int j)
{
	if (!t)
		return (0);
	i = 0;
	while (t[i])
	{
		if (ft_is_redirection(t[i]) && t[i + 1])
		{
			free(t[i]);
			free(t[i + 1]);
			j = i;
			while (t[j + 2])
			{
				t[j] = t[j + 2];
				j++;
			}
			t[j] = NULL;
			t[j + 1] = NULL;
			continue ;
		}
		i++;
	}
	return (1);
}

int	ft_is_redirection(char *str)
{
	return (!ft_strcmp(str, "<") || !ft_strcmp(str, ">") || !ft_strcmp(str,
			">>") || !ft_strcmp(str, "<<"));
}

void	ft_free_redirection(t_rdr *r)
{
	if (r->in_file)
	{
		free(r->in_file);
		r->in_file = NULL;
	}
	if (r->out_file)
	{
		free(r->out_file);
		r->out_file = NULL;
	}
	if (r->append_file)
	{
		free(r->append_file);
		r->append_file = NULL;
	}
	if (r->heredoc_delim)
	{
		free(r->heredoc_delim);
		r->heredoc_delim = NULL;
	}
}

int	ft_syntax_error(char *token)
{
	char	*mes1;
	char	*mes2;

	mes1 = "minishell: syntax error near unexpected token `";
	mes2 = "minishell: syntax error near unexpected token `newline'\n";
	if (token)
	{
		write(2, mes1, ft_strlen(mes1));
		write(2, token, ft_strlen(token));
		write(2, "'\n", 2);
	}
	else
		write(2, mes2, ft_strlen(mes2));
	return (0);
}

int	ft_check_redir(char **tokens, int i)
{
	if (!tokens[i + 1] || ft_is_redirection(tokens[i + 1])
		|| !ft_strcmp(tokens[i + 1], "|"))
		return (ft_syntax_error(tokens[i + 1]));
	return (1);
}

int	ft_check_pipe(char **tokens, int i)
{
	if (i == 0 || !tokens[i + 1] || ft_is_redirection(tokens[i + 1]))
		return (ft_syntax_error(tokens[i + 1]));
	return (1);
}

int	ft_validate_syntax(char **tokens)
{
	int i;

	if (!tokens || !tokens[0])
		return (0);
	i = 0;
	while (tokens[i])
	{
		if (ft_is_redirection(tokens[i]))
		{
			if (!ft_check_redir(tokens, i))
				return (0);
		}
		else if (!ft_strcmp(tokens[i], "|"))
		{
			if (!ft_check_pipe(tokens, i))
				return (0);
		}
		i++;
	}
	return (1);
}