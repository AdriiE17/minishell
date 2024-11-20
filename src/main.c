/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 10:30:57 by victor            #+#    #+#             */
/*   Updated: 2024/11/20 16:52:33 by vberdugo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

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

int	main(int argc, char *argv[], char *env[])
{
	char	*line;
	int		exit_status;
	int		i;
	char	*username;
	char	*hostname;
	char	cwd[PATH_MAX];
	char	prompt[PATH_MAX + HOST_NAME_MAX + 50];
	char	*expanded_line;
	char	*cmd_token;
	char	*sub_token;
	char	*cmd;
	int		pipefds[2];
	int		prev_pipefd;
	pid_t	pid;

	exit_status = 0;
	i = -1;
	if (argc > 1)
		while (++i < argc)
			if (ft_strcmp(argv[i], "--version") == 0)
				return (printf("Versión 1.0\n"), 0);
	while (1)
	{
		username = find_env_var("USER", env);
		hostname = find_env_var("HOSTNAME", env);
		if (!username)
			username = "user";
		if (!hostname)
			hostname = "localhost";
		if (!getcwd(cwd, sizeof(cwd)))
		{
			perror("getcwd");
			break ;
		}
		ft_strcpy(prompt, username);
		ft_strcat(prompt, "@");
		ft_strcat(prompt, hostname);
		ft_strcat(prompt, " - ");
		ft_strcat(prompt, cwd);
		ft_strcat(prompt, "$ ");
		line = readline(prompt);
		if (!line)
			break ;
		add_history(line);
		expanded_line = exp_env_vars(line, exit_status);
		cmd_token = strtok(expanded_line, ";");
		while (cmd_token != NULL)
		{
			sub_token = ft_strtok(cmd_token, "|");
			prev_pipefd = 0;
			while (sub_token != NULL)
			{
				cmd = sub_token;
				if (ft_strcmp(cmd, "exit") == 0)
				{
					printf("exit\n");
					free(expanded_line);
					free(line);
					exit(exit_status);
				}
				if (pipe(pipefds) == -1)
				{
					perror("pipe");
					return (1);
				}
				pid = fork();
				if (pid == 0)
				{
					if (prev_pipefd)
					{
						dup2(prev_pipefd, STDIN_FILENO);
						close(prev_pipefd);
					}
					if (strtok(NULL, "|") != NULL)
					{
						dup2(pipefds[1], STDOUT_FILENO);
					}
					close(pipefds[0]);
					close(pipefds[1]);
					if (ft_is_builtin(cmd))
						ft_execute(cmd, &exit_status);
					else
						ft_command(cmd, &exit_status);
					exit(0);
				}
				else if (pid < 0)
				{
					perror("fork");
					exit(1);
				}
				else
				{
					close(pipefds[1]);
					prev_pipefd = pipefds[0];
				}
				sub_token = ft_strtok(NULL, "|");
			}
			while (wait(NULL) > 0)
				;
			cmd_token = strtok(NULL, ";");
		}
		free(expanded_line);
		free(line);
	}
	return (0);
}
