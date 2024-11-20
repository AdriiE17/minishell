/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 10:14:01 by victor            #+#    #+#             */
/*   Updated: 2024/11/20 15:41:25 by vberdugo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef PARSE_H
# define PARSE_H

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

void	ft_command(char *cmd, int *exit_status);
char	*exp_env_vars(char *input, int exit_status);
int		ft_is_builtin(char *cmd);
void	ft_execute(char *cmd, int *exit_status);
int		count_pipes(char *str);

#endif
