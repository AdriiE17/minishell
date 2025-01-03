/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 11:15:41 by victor            #+#    #+#             */
/*   Updated: 2024/12/31 11:32:27 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_executable_path(char **paths, char *file)
{
	int		i;
	char	*temp;
	char	*path;

	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		path = ft_strjoin(temp, file);
		free(temp);
		if (access(path, F_OK | X_OK | R_OK) == 0)
			return (path);
		free(path);
		i++;
	}
	return (NULL);
}

void	free_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

char	*get_path_from_env(char **env)
{
	char	*path;
	int		i;

	path = NULL;
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
		{
			path = ft_strdup(env[i] + 5);
			break ;
		}
		i++;
	}
	return (path);
}

char	*find_command_in_path(char *file, char **env)
{
	char	**paths;
	char	*path;

	if (file[0] == '/' && access(file, F_OK | X_OK | R_OK) == 0)
	{
		path = ft_strdup(file);
		return (path);
	}
	path = get_path_from_env(env);
	if (!path)
		return (NULL);
	paths = ft_split(path, ':');
	free(path);
	path = find_executable_path(paths, file);
	free_array(paths);
	return (path);
}
