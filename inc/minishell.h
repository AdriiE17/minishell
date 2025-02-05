/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 10:14:01 by victor            #+#    #+#             */
/*   Updated: 2025/02/05 18:05:39 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef MINISHELL_H
# define MINISHELL_H

/* readline, rl_clear_history, rl_on_new_line, rl_replace_line, rl_redisplay */
# include <readline/readline.h>
/* add_history */
# include <readline/history.h>
/* printf */
# include <stdio.h>
/* malloc, free */
# include <stdlib.h>
/* write, access, open, read, close, unlink, execve, dup, dup2, pipe, isatty */
/* ttyname, ttyslot, getcwd, chdir */
# include <unistd.h>
/* open, O_WRONLY, O_CREAT, O_APPEND, O_TRUNC */
# include <fcntl.h>
/* fork, wait, waitpid, wait3, wait4 */
# include <sys/types.h>
# include <sys/wait.h>
/* signal, sigaction, kill */
# include <signal.h>
/* stat, lstat, fstat */
# include <sys/stat.h>
/* opendir, readdir, closedir */
# include <dirent.h>
/* strerror */
# include <string.h>
/* perror */
# include <stdio.h>
/* ioctl */
# include <sys/ioctl.h>
/* getenv */
# include <stdlib.h>
/* tcsetattr, tcgetattr */
# include <termios.h>
/* tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs */
# include <term.h>
# include "../libft/libft.h"
# include <stdbool.h>

# define D_QUOTE '"'
# define S_QUOTE '\''
# define S_PTR 8

extern volatile sig_atomic_t	g_signal_received;
typedef struct s_expan
{
	char	*expanded;
	char	*ptr;
	char	*cmd_copy;
	char	**arg;
	int		ind;
	int		in_single;
	int		in_double;
	char	quote_char;
}	t_expan;
typedef struct s_pipe
{
	int		pre_fd;
	int		pipefds[2];
	int		has_cmd;
	char	*sub_token;
	int		in_q;
	char	q_char;
}	t_pipe;

void	process_command(char *line, int *exit_status, char **env);
char	*handle_prompt(char *prompt, char *env[]);
int		print_version(int argc, char *argv[]);
void	ft_command(char *cmd, int *exit_status);
char	**split_args(const char *cmd);
void	construct_prompt(char *prompt, char *env[]);
bool	has_unclosed_quotes(const char *line);
char	*handle_multiline_input(char *initial_line);
/* parse */
char	*expand_all_env_vars(char *input, int exit_status);
/* built-ins */
int		ft_is_builtin(char *cmd);
void	ft_exec_builtin(char **cmd, int *exit_status, char **env);
void	ft_execute_cd(char **cmd);
void	ft_execute_echo(char **cmd, int *exit_status);
void	ft_execute_export(char **cmd, char **env);
int		ft_execute_unset(char **cmd, char **env);
void	ft_execute_env(char **env);
void	ft_execute_pwd(int *exit_status);
int		exe_echo(char **av);
int		assign_export_vars(char **av, char **env);
int		print_exported_vars(char **env);
bool	check_key(char *arg);
void	remove_env_variable(int index, char **env);
/* pipes */
t_expan	init_expan(const char *cmd);
void	handle_pipe_error(void);
void	handle_fork_error(void);
void	execute_and_handle_error(char **args, char **env);
void	close_pipe(int pipefd);
void	execute_pipeline(char *cmd, int *exit_status, char **env);
void	handle_child(char *sub_t, t_pipe *pipe_d, int *exit_s, char **env);
void	handle_pipe_redirection(int prev_pipe, int pipefds[2], int next_cmd);
int		validate_syntax(const char *cmd, int *exit_status);
char	*find_command_in_path(char *file, char **env);
/* signals */
void	signal_handler(int sig);
void	configure_terminal(void);
int		initialize_shell(int argc, char *argv[]);
int		handle_exit(char *line, int exit_status);
void	process_line(char *line, int *exit_status, char **env);
/* redirection */
void	process_string(char **s);
void	handle_redirections(char **args, int *exit_status, char **env);
int		handle_arguments(char **args, int exit_status);
int		handle_input_redirect(char **args, int exit_status, int *i, char **env);
int		handle_output_redirect(char **args, int exit_statu, int *i, char **env);
int		handle_here_doc_redirect(char **args, int exit_sta, int *i, char **env);
int		fork_heredoc_pipe(int pipe_fd[2], char *cmd, char *delimit, char **env);
int		handle_heredoc_in(int pipe_fd[2], char *cmd, char *delimit, char **env);

#endif
