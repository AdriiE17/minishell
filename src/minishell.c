/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 10:30:57 by victor            #+#    #+#             */
/*   Updated: 2024/11/27 19:42:12 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char *argv[], char *env[])
{
	char	*line;
	char	prompt[PATH_MAX + HOST_NAME_MAX + 50];
	int		exit_status;

	exit_status = 0;
	if (!print_version(argc, argv))
		return (0);
	while (1)
	{
		line = handle_prompt(prompt, env);
		if (!line)
			break ;
		add_history(line);
		if (ft_strcmp(line, "exit") == 0)
		{
			free(line);
			printf("exit\n");
			break ;
		}
		process_command(line, &exit_status);
		free(line);
	}
	return (exit_status);
}
