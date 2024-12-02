/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 10:18:35 by victor            #+#    #+#             */
/*   Updated: 2024/12/02 17:56:30 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	expand_exit_status(char *expanded, int i, int exit_status)
{
	char	*exit_str;
	int		j;

	exit_str = ft_itoa(exit_status);
	j = 0;
	while (exit_str[j])
		expanded[i++] = exit_str[j++];
	free(exit_str);
	return (i);
}

static int	expand_env_var(char *expanded, char **ptr, int i)
{
	char	*env_var;

	env_var = getenv(*ptr);
	if (env_var)
	{
		while (*env_var)
			expanded[i++] = *env_var++;
		while (**ptr && **ptr != ' ' && **ptr != '$')
			(*ptr)++;
	}
	else
	{
		(*ptr)++;
	}
	return (i);
}

/* ************************************************************************** */
/* Expands environment variables within the input string. If a '$' character  */
/* is found, it checks if it's followed by a valid environment variable or    */
/* the special '$?', replacing it with the corresponding value. The function  */
/* returns a new string with the expanded variables.                          */
/* ************************************************************************** */
char	*exp_env_vars(char *input, int exit_status)
{
	char	*expanded;
	char	*ptr;
	int		i;

	expanded = malloc(strlen(input) + 1);
	ptr = input;
	i = 0;
	while (*ptr)
	{
		if (*ptr == '$')
		{
			ptr++;
			if (*ptr == '?')
			{
				ptr++;
				i = expand_exit_status(expanded, i, exit_status);
			}
			else
				i = expand_env_var(expanded, &ptr, i);
		}
		else
			expanded[i++] = *ptr++;
	}
	expanded[i] = '\0';
	return (expanded);
}

/* ************************************************************************** */
/* Executes a non-built-in command by forking a new process. The command is   */
/* tokenized into arguments using strtok, and then executed with execvp in    */
/* the child process. The parent process waits for the child to finish and    */
/* updates the exit status. If an error occurs during forking or execution,   */
/* appropriate error messages are displayed.                                  */
/* ************************************************************************** */
void	ft_command(char *cmd, int *exit_status)
{
	pid_t	pid;
	char	*args[64];
	char	*token;
	int		i;

	i = 0;
	token = ft_strtok(cmd, " ");
	while (token != NULL)
	{
		args[i++] = token;
		token = ft_strtok(NULL, " ");
	}
	args[i] = NULL;
	pid = fork();
	if (pid == 0)
		execute_and_handle_error(args);
	else if (pid < 0)
		perror ("fork");
	else
	{
		waitpid(pid, exit_status, 0);
		*exit_status = WEXITSTATUS(*exit_status);
	}
}
