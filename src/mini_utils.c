/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 19:39:05 by victor            #+#    #+#             */
/*   Updated: 2024/12/02 21:31:15 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ************************************************************************** */
/* Searches for the value of an environment variable in the env array.        */
/* Returns a pointer to the value if found, or NULL if not found.             */
/* ************************************************************************** */
char	*find_env_var(char *name, char *env[])
{
	int	i;
	int	len;

	len = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, len) == 0 && env[i][len] == '=')
			return (&env[i][len + 1]);
		i++;
	}
	return (NULL);
}

/* ************************************************************************** */
/* Constructs the shell prompt using the USER and HOSTNAME environment        */
/* variables. If these are not set, default values are used. The current      */
/* working directory is appended to complete the prompt.                      */
/* ************************************************************************** */
void	construct_prompt(char *prompt, char *env[])
{
	char	*username;
	char	*hostname;
	char	cwd[PATH_MAX];

	username = find_env_var("USER", env);
	hostname = find_env_var("HOSTNAME", env);
	if (!username)
		username = "user";
	if (!hostname)
		hostname = "localhost";
	if (!getcwd(cwd, sizeof(cwd)))
	{
		perror("getcwd");
		strcpy(prompt, "prompt_error$ ");
		return ;
	}
	ft_strcpy(prompt, username);
	ft_strcat(prompt, "@");
	ft_strcat(prompt, hostname);
	ft_strcat(prompt, " - ");
	ft_strcat(prompt, cwd);
	ft_strcat(prompt, "$ ");
}

/* ************************************************************************** */
/* Checks if the --version flag is passed as an argument. If present,         */
/* prints the version information and returns 0. Otherwise, returns 1.        */
/* ************************************************************************** */
int	print_version(int argc, char *argv[])
{
	if (argc > 1 && ft_strcmp(argv[1], "--version") == 0)
	{
		printf("Versión 1.0\n");
		return (0);
	}
	return (1);
}

/* ************************************************************************** */
/* Generates a dynamic shell prompt and waits for user input. Returns the     */
/* input string from the user.                                                */
/* ************************************************************************** */
char	*handle_prompt(char *prompt, char *env[])
{
	char	*line;

	write(STDOUT_FILENO, "\r", 1);
	construct_prompt(prompt, env);
	g_signal_received = 0;
	line = readline(prompt);
	if (line != NULL && isatty(STDIN_FILENO))
	{
		rl_redisplay();
	}
	return (line);
}

/* ************************************************************************** */
/* Processes the input command by expanding environment variables, splitting  */
/* it into tokens by semicolons, and executing each pipeline of commands.     */
/* ************************************************************************** */
void	process_command(char *line, int *exit_status)
{
	char	*expanded_line;
	char	*cmd_token;

	expanded_line = exp_env_vars(line, *exit_status);
	cmd_token = ft_strtok(expanded_line, ";");
	while (cmd_token != NULL)
	{
		execute_pipeline(cmd_token, exit_status);
		cmd_token = ft_strtok(NULL, ";");
	}
	free(expanded_line);
}
