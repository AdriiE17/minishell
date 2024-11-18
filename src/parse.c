/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 10:18:35 by victor            #+#    #+#             */
/*   Updated: 2024/11/18 17:34:18 by vberdugo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

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
	char	*env_var;
	int		i;
	int		j;
	int		num_len;
	char	str_exit_status[20];
	int		temp;
	char	tmp;

	expanded = malloc(ft_strlen(input) + 1);
	ptr = input;
	i = 0;
	while (*ptr)
	{
		if (*ptr == '$')
		{
			ptr++;
			if (*ptr == '?')
			{
				temp = exit_status;
				num_len = 0;
				if (temp == 0)
					str_exit_status[num_len++] = '0';
				else
				{
					while (temp > 0)
					{
						str_exit_status[num_len++] = (temp % 10) + '0';
						temp /= 10;
					}
					j = -1;
					while (++j < num_len / 2)
					{
						tmp = str_exit_status[j];
						str_exit_status[j] = str_exit_status[num_len - j - 1];
						str_exit_status[num_len - j - 1] = tmp;
					}
				}
				str_exit_status[num_len] = '\0';
				j = -1;
				while (++j < num_len)
					expanded[i++] = str_exit_status[j];
				ptr++;
			}
			else
			{
				env_var = getenv(ptr);
				if (env_var)
				{
					while (*env_var)
						expanded[i++] = *env_var++;
					while (*ptr && *ptr != ' ' && *ptr != '$')
						ptr++;
				}
				else
					ptr++;
			}
		}
		else
			expanded[i++] = *ptr++;
	}
	expanded[i] = '\0';
	return (expanded);
}

/* ************************************************************************** */
/* Determines if the given command is a built-in shell command. It compares   */
/* the input string with a predefined list of built-in commands and returns   */
/* 1 if it's a built-in command, otherwise returns 0.                         */
/* ************************************************************************** */
int	ft_is_builtin(char *cmd)
{
	if (strcmp(cmd, "echo") == 0 || strcmp(cmd, "cd") == 0
		|| strcmp(cmd, "pwd1") == 0 || strcmp(cmd, "export") == 0
		|| strcmp(cmd, "unset") == 0 || strcmp(cmd, "env") == 0
		|| strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
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
	{
		if (execvp(args[0], args) == -1)
		{
			perror("execvp");
			exit(EXIT_FAILURE);
		}
	}
	else if (pid < 0)
	{
		perror ("fork");
	}
	else
	{
		waitpid(pid, exit_status, 0);
		*exit_status = WEXITSTATUS(*exit_status);
	}
}

/* ************************************************************************** */
/* Handles the execution of built-in shell commands like cd, exit, echo,      */
/* pwd, export, unset, and env. Based on the command, it performs the         */
/* appropriate action, such as changing the directory, printing the current   */
/* directory, setting/unsetting environment variables, or printing environment*/
/* variables. The exit status is updated accordingly for each command.        */
/* ************************************************************************** */
void	ft_execute(char *cmd, int *exit_status)
{
	char		*path;
	char		*arg;
	char		*var;
	char		*value;
	extern char	**environ;
	char		cwd[1024];
	int			i;

	if (ft_strcmp(cmd, "cd") == 0)
	{
		path = ft_strtok(NULL, " ");
		if (path == NULL || chdir(path) == -1)
		{
			perror("cd");
			*exit_status = 1;
		}
		else
		{
			*exit_status = 0;
		}
	}
	else if (ft_strcmp(cmd, "exit") == 0)
	{
		exit(*exit_status);
	}
	else if (ft_strcmp(cmd, "echo") == 0)
	{
		arg = ft_strtok(NULL, " ");
		while (arg != NULL)
		{
			write(STDOUT_FILENO, arg, ft_strlen(arg));
			write(STDOUT_FILENO, " ", 1);
			arg = strtok(NULL, " ");
		}
		write(STDOUT_FILENO, "\n", 1);
		*exit_status = 0;
	}
	else if (ft_strcmp(cmd, "pwd") == 0)
	{
		if (getcwd(cwd, sizeof(cwd)) != NULL)
		{
			write(STDOUT_FILENO, cwd, ft_strlen(cwd));
			write(STDOUT_FILENO, "\n", 1);
			*exit_status = 0;
		}
		else
		{
			perror("pwd");
			*exit_status = 1;
		}
	}
	else if (ft_strcmp(cmd, "export") == 0)
	{
		var = ft_strtok(NULL, " ");
		if (var)
		{
			value = ft_strtok(NULL, " ");
			if (value)
			{
				setenv(var, value, 1);
				*exit_status = 0;
			}
			else
			{
				write(STDERR_FILENO, "export: invalid argument\n", 25);
				*exit_status = 1;
			}
		}
	}
	else if (ft_strcmp(cmd, "unset") == 0)
	{
		var = ft_strtok(NULL, " ");
		if (var)
		{
			unsetenv(var);
			*exit_status = 0;
		}
		else
		{
			write(STDERR_FILENO, "unset: invalid argument\n", 24);
			*exit_status = 1;
		}
	}
	else if (ft_strcmp(cmd, "env") == 0)
	{
		i = -1;
		while (environ[++i] != NULL)
		{
			write(STDOUT_FILENO, environ[i], ft_strlen(environ[i]));
			write(STDOUT_FILENO, "\n", 1);
		}
		*exit_status = 0;
	}
}
