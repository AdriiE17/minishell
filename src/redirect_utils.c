/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 11:28:43 by victor            #+#    #+#             */
/*   Updated: 2024/12/16 15:52:43 by vberdugo         ###   ########.fr       */
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
			fprintf(stderr, "Error: failed to expand variables\n");
			return (1);
		}
		free(args[i]);
		args[i] = expanded;
		i++;
	}
	return (exit_status);
}

/* ************************************************************************** */
/* Handles input redirection using a pipe and a delimiter. It continuously    */
/* reads input from the user until the delimiter is entered, writing the      */
/* input to the pipe. The function exits after closing the pipe.              */
/* ************************************************************************** */
int	handle_here_doc_input(int pipe_fd[2], char *delimiter)
{
	char	*line;
	size_t	len;

	line = NULL;
	len = 0;
	while (1)
	{
		printf("> ");
		if (getline(&line, &len, stdin) == -1)
			break ;
		line[strcspn(line, "\n")] = '\0';
		if (strcmp(line, delimiter) == 0)
			break ;
		write(pipe_fd[1], line, strlen(line));
		write(pipe_fd[1], "\n", 1);
	}
	free(line);
	close(pipe_fd[1]);
	exit(0);
}

/* ************************************************************************** */
/* Handles input redirection by opening the specified file and redirecting    */
/* the input stream to that file using dup2. If the file can't be opened, an  */
/* error message is printed and the function returns 1.                       */
/* ************************************************************************** */
int	handle_input_redirect(char **args, int exit_status, int *i)
{
	char	*clean_filename;
	int		fd_in;

	clean_filename = remove_quotes(args[*i + 1]);
	fd_in = open(clean_filename, O_RDONLY);
	free(clean_filename);
	if (fd_in == -1)
	{
		perror(args[*i + 1]);
		return (1);
	}
	if (dup2(fd_in, STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(fd_in);
		return (1);
	}
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
int	handle_output_redirect(char **args, int exit_status, int *i)
{
	int		fd_out;
	int		flags;

	process_string(args);
	if (strcmp(args[*i], ">>") == 0)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	fd_out = open(args[*i + 1], flags, 0644);
	if (fd_out == -1)
		return (perror(args[*i + 1]), 1);
	if (dup2(fd_out, STDOUT_FILENO) == -1)
		return (perror("dup2"), close(fd_out), 1);
	close(fd_out);
	args[*i] = NULL;
	args[*i + 1] = NULL;
	(*i)++;
	return (exit_status);
}

/* ************************************************************************** */
/* Handles here document redirection by creating a pipe and forking a child   */
/* process. The child process reads input from the user until the delimiter   */
/* is encountered and writes the input to the pipe. The parent process waits  */
/* for the child to finish and then redirects the input stream to the pipe.   */
/* ************************************************************************** */
int	handle_here_doc_redirect(char **args, int exit, int *i)
{
	char	*delimite;
	int		pipe_fd[2];
	pid_t	pid;

	delimite = remove_quotes(args[*i + 1]);
	if (pipe(pipe_fd) == -1)
		return (perror("pipe"), free(delimite), 1);
	pid = fork();
	if (pid == 0)
	{
		close(pipe_fd[0]);
		handle_here_doc_input(pipe_fd, delimite);
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
}
