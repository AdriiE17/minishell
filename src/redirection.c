/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vberdugo <vberdugo@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 12:07:58 by vberdugo          #+#    #+#             */
/*   Updated: 2025/01/22 20:18:30 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ************************************************************************** */
/* Handles input redirection ('<') by checking if the next argument is a file */
/* and calling the corresponding function to perform the redirection. If the  */
/* filename is missing, it prints an error message and sets the exit status.  */
/* ************************************************************************** */
void	input_redirection(char **args, int *exit_status, int *i, char **env)
{
	if (args[*i + 1])
		*exit_status = handle_input_redirect(args, *exit_status, i, env);
	else
	{
		printf("Error: missing filename after '<'\n");
		*exit_status = 1;
	}
}

/* ************************************************************************** */
/* Handles output redirection ('>' or '>>') by checking if the next argument  */
/* is a file and calling the corresponding function to perform the redirection*/
/* If the filename is missing, it prints an error message and sets the exit   */
/* status.                                                                    */
/* ************************************************************************** */
void	output_redirection(char **args, int *exit_status, int *i, char **env)
{
	if (args[*i + 1])
		*exit_status = handle_output_redirect(args, *exit_status, i, env);
	else
	{
		printf("Error: missing file after '%s'\n", args[*i]);
		*exit_status = 1;
	}
}

/* ************************************************************************** */
/* Handles here document redirection ('<<') by checking if the next argument  */
/* is a delimiter and calling the corresponding function to perform the redir-*/
/* ection. If the delimiter is missing, it prints an error message and sets   */
/* the exit status.                                                           */
/* ************************************************************************** */
void	handle_here_doc(char **args, int *exit_status, int *i, char **env)
{
	if (args[*i + 1])
		*exit_status = handle_here_doc_redirect(args, *exit_status, i, env);
	else
	{
		printf("Error: missing delimiter after '<<'\n");
		*exit_status = 1;
	}
}

int	fork_heredoc_pipe(int pipe_fd[2], char *cmd, char *delimit, char **env)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		close(pipe_fd[0]);
		handle_heredoc_in(pipe_fd, cmd, delimit, env);
		free(delimit);
		exit(0);
	}
	else if (pid > 0)
	{
		close(pipe_fd[1]);
		if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
			return (perror("dup2"), close(pipe_fd[0]), free(delimit), -1);
		close(pipe_fd[0]);
		waitpid(pid, NULL, 0);
	}
	else
		return (perror("fork"), free(delimit), -1);
	return (0);
}

/* ************************************************************************** */
/* Loops through the arguments and checks for redirection operators ('<',     */
/* '>', '>>', '<<'). For each type of redirection operator, it calls the      */
/* corresponding handler function (handle_input_redirection, handle_output_   */
/* redirection, or handle_here_doc_redirection). It increments the index to   */
/* process all arguments.                                                     */
/* ************************************************************************** */
void	handle_redirections(char **args, int *exit_status, char **env)
{
	int	i;

	i = 0;
	while (args[i])
	{
		if (ft_strcmp(args[i], "<") == 0)
			input_redirection(args, exit_status, &i, env);
		else if (ft_strcmp(args[i], ">") == 0 || ft_strcmp(args[i], ">>") == 0)
			output_redirection(args, exit_status, &i, env);
		else if (ft_strcmp(args[i], "<<") == 0)
			handle_here_doc(args, exit_status, &i, env);
		i++;
	}
	signal(SIGQUIT, signal_handler);
}
