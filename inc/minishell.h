/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 10:14:01 by victor            #+#    #+#             */
/*   Updated: 2024/12/11 20:10:35 by victor           ###   ########.fr       */
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

extern volatile sig_atomic_t	g_signal_received;
typedef struct s_expansion
{
	char	*expanded;
	char	*ptr;
	int		index;
	int		in_single;
	int		in_double;
}	t_expansion;

void	process_command(char *line, int *exit_status);
char	*handle_prompt(char *prompt, char *env[]);
int		print_version(int argc, char *argv[]);
void	ft_command(char *cmd, int *exit_status);
char	**split_args(const char *cmd);
void	construct_prompt(char *prompt, char *env[]);
/* parse */
char	*exp_env_vars(char *input, int exit_status);
void	strip_single_quotes(char *str);
void	strip_double_quotes(char *str);
void	strip_quotes(char *str);
/* built-ins */
int		ft_is_builtin(char *cmd);
void	ft_exec_builtin(char *cmd, int *exit_status);
void	ft_execute_cd(int *exit_status);
void	ft_execute_echo(int *exit_status);
void	ft_execute_export(int *exit_status);
void	ft_execute_unset(int *exit_status);
void	ft_execute_env(int *exit_status);
void	ft_execute_pwd(int *exit_status);
/* pipes */
int		count_pipes(char *str);
void	handle_pipe_error(void);
void	handle_fork_error(void);
void	execute_and_handle_error(char **args);
void	close_pipe(int pipefd);
void	execute_pipeline(char *cmd, int *exit_status);
//void	handle_child(char *sub_t, int prev_fd, int pipefds[2], int *exit_s);
void    handle_child(char *sub_t, int prev_fd, int pipefds[2], int has_next_cmd, int *exit_s);
//void	handle_pipe_redirection(int prev_pipefd, int pipefds[2]);
void    handle_pipe_redirection(int prev_pipefd, int pipefds[2], int has_next_cmd);
/* signals */
void	signal_handler(int sig);
void	configure_terminal(void);
int		initialize_shell(int argc, char *argv[]);
int		handle_exit(char *line, int exit_status);
void	process_line(char *line, int *exit_status);
/* redirection */
int		handle_arguments(char **args, int exit_status);
int		handle_input_redirect(char **args, int exit_status, int *i);
int		handle_output_redirect(char **args, int exit_status, int *i);
int		handle_here_doc_redirect(char **args, int exit_status, int *i);
void	handle_redirections(char **args, int *exit_status);

#endif
