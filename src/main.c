/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 10:30:57 by victor            #+#    #+#             */
/*   Updated: 2024/11/27 12:07:29 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

char **split_args(const char *cmd) {
	int count = 0;
	char **args = NULL;
	char *token = NULL;
	char *cmd_copy = ft_strdup(cmd);
	char *ptr = cmd_copy;

	if (!cmd_copy)
		return NULL;

	while ((token = strsep(&ptr, " ")) != NULL) {
		if (*token == '\0')
			continue;
		args = realloc(args, sizeof(char *) * (count + 1));
		args[count++] = strdup(token);
	}
	args = realloc(args, sizeof(char *) * (count + 1));
	args[count] = NULL;
	free(cmd_copy);
	return args;
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
	//char	*cmd_token;
	//char	*sub_token;
	//char	*cmd;
	//int		pipefds[2];
	//int		prev_pipefd;
	//pid_t	pid;

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
// Expandir variables de entorno
		if (ft_strcmp(line, "exit") == 0)
		{
			free(line);
			printf("exit\n");
			break;
		}
        expanded_line = exp_env_vars(line, exit_status);

        // Tokenizar por comandos separados por ";"
        char *cmd_token = ft_strtok(expanded_line, ";");
        while (cmd_token != NULL) {
            // Manejar cada comando con pipes
            execute_pipeline(cmd_token, &exit_status);
            cmd_token = strtok(NULL, ";");
        }
		free(expanded_line);
		free(line);
	}
	return (0);
}
