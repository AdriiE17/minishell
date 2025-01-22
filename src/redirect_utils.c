/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 11:28:43 by victor            #+#    #+#             */
/*   Updated: 2025/01/22 20:15:44 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ************************************************************************** */
/* Expands environment variables for each argument in the args array. If any  */
/* expansion fails, an error message is printed and the function returns 1.   */
/* Each argument is updated with its expanded value.                          */
/* ************************************************************************** */
int	handle_arguments(char **args, int exit_status)
{
	int		i;
	char	*expanded;

	i = 0;
	while (args[i])
	{
		expanded = expand_all_env_vars(args[i], exit_status);
		if (!expanded)
		{
			printf("Error: failed to expand variables\n");
			return (1);
		}
		free(args[i]);
		args[i] = expanded;
		i++;
	}
	return (exit_status);
}

/* ************************************************************************** */
/* Handles input redirection by opening the specified file and redirecting    */
/* the input stream to that file using dup2. If the file can't be opened, an  */
/* error message is printed and the function returns 1.                       */
/* ************************************************************************** */
int	handle_input_redirect(char **args, int exit_status, int *i, char **env)
{
	int		fd_in;
	char	*filename;
	char	*command_path;

	command_path = find_command_in_path(args[0], env);
	if (!command_path)
		return (printf("%s: command not found\n", args[0]), 1);
	free(command_path);
	filename = strdup(args[*i + 1]);
	if (!filename)
		return (perror("strdup"), 1);
	process_string(&filename);
	fd_in = open(filename, O_RDONLY);
	free(filename);
	if (fd_in == -1)
		return (perror(args[*i + 1]), 1);
	if (dup2(fd_in, STDIN_FILENO) == -1)
		return (perror("dup2"), close(fd_in), 1);
	close(fd_in);
	args[*i] = NULL;
	args[*i + 1] = NULL;
	(*i)++;
	return (exit_status);
}

/* ************************************************************************** */
/* Handles output redirection by opening the specified file and redirecting   */
/* the output stream to that file. It also handles appending ('>>') or        */
/* overwriting ('>') based on the redirection type. If the file can't be      */
/* opened, an error message is printed and the function returns 1.            */
/* ************************************************************************** */
int	handle_output_redirect(char **args, int exit_statu, int *i, char **env)
{
	int		fd_out;
	int		flags;
	char	*filename;
	char	*command_path;

	command_path = find_command_in_path(args[0], env);
	if (!command_path)
		return (printf("%s: command not found\n", args[0]), 1);
	free(command_path);
	filename = strdup(args[*i + 1]);
	if (!filename)
		return (perror("strdup"), 1);
	process_string(&filename);
	if (strcmp(args[*i], ">>") == 0)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	fd_out = open(filename, flags, 0644);
	free(filename);
	if (fd_out == -1)
		return (perror(args[*i + 1]), 1);
	if (dup2(fd_out, STDOUT_FILENO) == -1)
		return (perror("dup2"), close(fd_out), 1);
	close(fd_out);
	return (args[*i] = NULL, args[*i + 1] = NULL, (*i)++, exit_statu);
}

/* ************************************************************************** */
/* Handles input redirection using a pipe and a delimiter. It continuously    */
/* reads input from the user until the delimiter is entered, writing the      */
/* input to the pipe. The function exits after closing the pipe.              */
/* ************************************************************************** */
int	handle_heredoc_in(int pipe_fd[2], char *cmd, char *delimit, char **env)
{
	char	*line;
	char	*command_path;

	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strcmp(line, delimit) == 0)
		{
			free(line);
			break ;
		}
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	command_path = find_command_in_path(cmd, env);
	if (!command_path)
		printf("%s: command not found\n", cmd);
	free(command_path);
	close(pipe_fd[1]);
	exit(0);
}

/* ************************************************************************** */
/* Handles here document redirection by creating a pipe and forking a child   */
/* process. The child process reads input from the user until the delimiter   */
/* is encountered and writes the input to the pipe. The parent process waits  */
/* for the child to finish and then redirects the input stream to the pipe.   */
/* ************************************************************************** *
int	handle_here_doc_redirect(char **args, int exit, int *i, char **env)
{
	char	*delimite;
	int		pipe_fd[2];
	pid_t	pid;
	char	*command_path;

	command_path = find_command_in_path(args[0], env);
	if (!command_path)
		return (handle_heredoc_in(pipe_fd, args[*i - 1], args[*i + 1], env), 1);
	free(command_path);
	delimite = args[*i + 1];
	if (pipe(pipe_fd) == -1)
		return (perror("pipe"), free(delimite), 1);
	pid = fork();
	if (pid == 0)
	{
		close(pipe_fd[0]);
		handle_heredoc_in(pipe_fd, args[*i - 1], delimite, env);
		free(delimite);
	}
	else if (pid > 0)
	{
		close(pipe_fd[1]);
		if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
			return (perror("dup2"), close(pipe_fd[0]), free(delimite), 1);
		close(pipe_fd[0]);
		waitpid(pid, NULL, 0);
	}
	else
		return (perror("fork"), free(delimite), 1);
	return (free(delimite), args[*i] = NULL, args[*i + 1] = NULL, (*i)++, exit);
}*/

int	handle_here_doc_redirect(char **args, int exit, int *i, char **env)
{
	char	*delimite;
	int		pipe_fd[2];
	char	*command_path;

	command_path = find_command_in_path(args[0], env);
	if (!command_path)
		return (handle_heredoc_in(pipe_fd, args[*i - 1], args[*i + 1], env), 1);
	free(command_path);
	delimite = args[*i + 1];
	if (pipe(pipe_fd) == -1)
		return (perror("pipe"), free(delimite), 1);
	if (fork_heredoc_pipe(pipe_fd, args[*i - 1], delimite, env) == -1)
		return (1);
	return (free(delimite), args[*i] = NULL, args[*i + 1] = NULL, (*i)++, exit);
}
