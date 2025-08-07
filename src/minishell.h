/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagwu <dagwu@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 18:48:53 by dagwu             #+#    #+#             */
/*   Updated: 2025/08/07 09:27:22 by daflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "libft.h"
# include <errno.h>
# include <fcntl.h>
# include <math.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <strings.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>

typedef struct s_dat
{
	struct s_variable_node	*av;
	struct s_variable_node	*ev;
	struct s_variable_node	*lo;
	char					**ln;
	char					**xln;
	char					*tmp1;
	char					*tmp2;
	size_t					i;
	size_t					j;
	size_t					k;
	size_t					tot;
	char					**avs;
	char					**evs;
}							t_dat;

typedef struct s_variable_node
{
	char					*name;
	char					*value;
	struct s_variable_node	*next;
}							t_va;

typedef struct s_redirections
{
	char					*in_file;
	char					*out_file;
	char					*append_file;
	char					*heredoc_delim;
}							t_rdr;

void						ft_free_string_array(char **str_array);
int							ft_increment_shlvl(t_va *node);
int							ft_create_shlvl(t_va **env_list);
void						ft_update_shlvl(t_va **env_list);
t_dat						ft_duplicate_input_args(int argc, char **argv,
								char **env);
void						ft_cleanup_data(t_dat *data);
void						ft_cleanup_exit(t_dat *data, int flag);
t_va						*create_lst_frm_arr(char **arr, t_va *h, int i,
								t_va *(*f)(char *));
t_va						*ft_create_node(char *str);
t_va						*ft_create_var_node(char *str);
char						*ft_extract_var_name(char *str);
char						*ft_extract_var_value(char *str, char quote,
								size_t len);
void						ft_free_list(t_va *head);
int							ft_strisspace(char *str);
void						ft_sigint_handler(int sig);
void						ft_set_main_signals(void);
void						ft_set_default_signals(void);
int							ft_skip_quote(char *str, int i);
int							ft_skip_token(char *str, int i);
int							ft_count_tokens(char *str);
int							ft_get_token_end(char *str, int i);
void						ft_detect_quote_type(char *token, int *quote_type);
char						*ft_extract_token(char *str, t_dat *d,
								int *quote_type);
void						ft_reset_iterators(t_dat *data);
char						**ft_free_token_quote(char **tokens,
								int *quote_types);
char						**ft_tokenize_line(t_dat *d, char *str,
								int **quote_types_out);
char						*ft_get_var_value(t_va *list, const char *name);
char						*ft_extract_var_key(const char *str, size_t *i);
char						*ft_strjoin_char(const char *s, char c);
void						ft_expand_loop(char *token, t_dat *data, char **res,
								size_t *i);
char						*ft_expand_token(char *token, t_dat *data, int qt,
								size_t i);
char						**ft_expand_tokens(t_dat *d, char **tokens,
								int *qtypes, int i);
void						ft_strip_surrounding_quotes(char *s);
void						ft_strip_quotes_after_equal(char *s);
void						ft_strip_quotes_from_xln(t_dat *d);
int							ft_valid_var(char *str);
t_va						*ft_find_var(t_va *list, const char *name);
int							ft_update_var_value(t_va *node, const char *value);
int							ft_add_local_var(t_dat *data, char *str);
void						ft_update_local_variables(t_dat *d);
int							ft_all_valid_lvar(t_dat *data, char **arr);
int							ft_is_number(const char *str);
char						*ft_get_val_from_list(t_va *head, const char *key);
int							ft_update_existing_var(t_va *node, const char *name,
								const char *val);
void						ft_create_env_variable(t_dat *d, const char *name,
								const char *value);
void						ft_update_env_variable(t_dat *d, const char *name,
								const char *value);
void						ft_pwd(void);
void						ft_update_directories(t_dat *data, char *oldpwd);
void						ft_cd_error(char *path);
void						ft_change_directory(t_dat *data, size_t k);
void						ft_echo(t_dat *d, size_t k);
void						ft_exit_numeric_error(char *arg);
void						ft_exit(t_dat *data, size_t k);
void						ft_env(t_dat *data);
void						ft_unset_multi_var(t_dat *d, size_t k);
t_va						*ft_remove_variable_node(const char *key,
								t_va *head, t_va *prev);
int							ft_var_name_only(char *str);
void						ft_append_env_var(t_dat *data, char *key,
								char *value);
void						ft_export_type2(t_dat *data, char *str);
t_va						*ft_export_type1_ext(char *name, char *val);
void						ft_export_type1(t_va **head, char *s, char *name,
								char *val);
void						ft_print_export(t_va *head);
t_va						*ft_merge_sorted_lists(t_va *a, t_va *b);
void						ft_split_list(t_va *source, t_va **front,
								t_va **back);
void						ft_sort_list_by_name(t_va **head_ref);
t_va						*ft_duplicate_node(const t_va *node);
int							ft_append_dup_node(const t_va *cur, t_va **head,
								t_va **tail);
t_va						*ft_duplicate_list(const t_va *head);
void						ft_print_sorted_env(t_va *head);
void						ft_export_error(char *arg, char *message);
void						ft_export_multi_var(t_dat *data, size_t k);
void						ft_handle_builtin(t_dat *data, char *line,
								size_t k);
void						ft_check_var_assign_and_expand_line(t_dat *data,
								char *line);
int							ft_count_list(t_va *head);
void						ft_list_to_env_array(t_dat *data);
char						*ft_join_path(char *str1, char *cmd);
char						*ft_get_cmd_path(t_dat *d, const char *cmd, int i);
int							ft_count_pipes(char **tokens);
void						ft_cmd_not_found(char *cmd);
void						ft_ex_single_cmd(t_dat *d, char *cmd_path);
void						ft_external_functions(t_dat *data, char *line);
char						**ft_extract_tokens(t_dat *data, int start,
								int end);
char						***ft_clean_cmd(char ***cmd);
int							ft_parse_cmd_helper(t_dat *d, char ***cmd, int *idx,
								int *st_i);
char						***ft_parse_cmd(t_dat *d, int st, int i, int idx);
void						ft_free_fd(int **fd);
int							**init_fd_array(int tot);
int							ft_create_pipes(int **fd, int tot);
void						ft_setup_io(int **fd, size_t i, size_t total);
void						ft_exec_command(t_dat *d, char **cmd);
void						ft_child_process(t_dat *d, char ***cmd, int **fd,
								size_t i);
int							ft_syntax_error_msg(char *token);
int							ft_validate_segment(char **tokens, int start,
								int end);
void						ft_fork_children(t_dat *d, char ***cmd, int **fd);
void						ft_close_pipes(int **fd, int tot);
void						ft_wait_children(int tot);
void						ft_execute_pipeline(t_dat *d, char ***cmd);
int							ft_parse_redirection(char **tokens, t_rdr *r);
int							ft_redir_in(char *file);
int							ft_redir_out(char *file);
int							ft_redir_append(char *file);
int							ft_handle_heredoc(char *delim, char *line);
int							ft_apply_sing_redirections(t_rdr *r, char **tok);
int							ft_apply_redirections(t_rdr *r, char ***cmd);
int							ft_remove_redirections(char ***tokens_ptr, int i,
								int j);
int							ft_remove_sing_redirections(char **t, int i, int j);
int							ft_is_redirection(char *str);
void						ft_free_redirection(t_rdr *r);
int							ft_syntax_error(char *token);
int							ft_check_redir(char **tokens, int i);
int							ft_check_pipe(char **tokens, int i);
int							ft_validate_syntax(char **tokens);

#endif
