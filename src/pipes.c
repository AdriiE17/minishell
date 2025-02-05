/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vberdugo <vberdugo@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 15:38:32 by vberdugo          #+#    #+#             */
/*   Updated: 2025/02/05 17:59:50 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_pipe_redirection(int prev_pipe, int pipefds[2], int next_cmd)
{
	if (prev_pipe != -1)
	{
		dup2(prev_pipe, STDIN_FILENO);
		close(prev_pipe);
	}
	if (next_cmd)
		dup2(pipefds[1], STDOUT_FILENO);
	close(pipefds[0]);
	close(pipefds[1]);
}

void	handle_child(char *sub_t, t_pipe *pipe_d, int *exit_s, char **env)
{
	char	**args;
	int		i;

	args = split_args(sub_t);
	if (!args || !args[0])
	{
		if (args)
			free(args);
		exit(*exit_s);
	}
	i = -1;
	while (args[++i])
		process_string(&args[i]);
	handle_pipe_redirection(pipe_d->pre_fd, pipe_d->pipefds, pipe_d->has_cmd);
	handle_redirections(args, exit_s, env);
	execute_and_handle_error(args, env);
	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

int	execute_command(t_pipe *pip, int *exit_status, char **env)
{
	pid_t	pid;
	char	**args;
	int		i;

	if (pipe(pip->pipefds) == -1)
		handle_pipe_error();
	args = split_args(pip->sub_token);
	if (!args)
		return (1);
	if (ft_is_builtin(args[0]))
		return (ft_exec_builtin(args, exit_status, env), 0);
	i = -1;
	while (args[++i])
		free(args[i]);
	free(args);
	pid = fork();
	if (pid == 0)
		handle_child(pip->sub_token, pip, exit_status, env);
	else if (pid < 0)
		handle_fork_error();
	close_pipe(pip->pre_fd);
	close(pip->pipefds[1]);
	pip->pre_fd = pip->pipefds[0];
	return (0);
}

void	handle_pipeline(char *cmd, int *exit_status, t_pipe *pip, char **env)
{
	if (validate_syntax(cmd, exit_status))
		return ;
	pip->in_q = 0;
	pip->q_char = '\0';
	pip->sub_token = ft_strsep(&cmd, "|", &pip->in_q, &pip->q_char);
	while (pip->sub_token != NULL)
	{
		pip->has_cmd = (cmd != NULL && *cmd != '\0');
		execute_command(pip, exit_status, env);
		if (pip->has_cmd)
			pip->sub_token = ft_strsep(&cmd, "|", &pip->in_q, &pip->q_char);
		else
			break ;
	}
}

void	execute_pipeline(char *cmd, int *exit_status, char **env)
{
	t_pipe	pipe;

	pipe.pre_fd = -1;
	handle_pipeline(cmd, exit_status, &pipe, env);
	close_pipe(pipe.pre_fd);
	signal(SIGQUIT, SIG_IGN);
	while (wait(NULL) > 0)
		;
}
