/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 13:56:07 by victor            #+#    #+#             */
/*   Updated: 2024/11/28 14:03:27 by vberdugo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ************************************************************************** */
/* Counts the number of pipe ('|') characters in a string and returns the     */
/* total number of commands in the pipeline.                                  */
/* ************************************************************************** */
int	count_pipes(char *str)
{
	int	count;

	count = 0;
	while (*str)
	{
		if (*str == '|')
		{
			count++;
		}
		str++;
	}
	return (count + 1);
}

/* ************************************************************************** */
/* Closes the given pipe file descriptor if it is valid.                      */
/* ************************************************************************** */
void	close_pipe(int pipefd)
{
	if (pipefd != -1)
	{
		close(pipefd);
	}
}

/* ************************************************************************** */
/* Handles an error in pipe creation by printing an error message and exiting.*/
/* ************************************************************************** */
void	handle_pipe_error(void)
{
	perror("pipe");
	exit(EXIT_FAILURE);
}

/* ************************************************************************** */
/* Handles an error during process forking by printing an error message and  */
/* exiting the program.                                                       */
/* ************************************************************************** */
void	handle_fork_error(void)
{
	perror("fork");
	exit(EXIT_FAILURE);
}

/* ************************************************************************** */
/* Attempts to execute an external command using execvp. If execution fails,  */
/* prints an error message and exits the process.                             */
/* ************************************************************************** */
void	execute_and_handle_error(char **args)
{
	if (execvp(args[0], args) == -1)
	{
		perror("execvp");
		exit(EXIT_FAILURE);
	}
}
