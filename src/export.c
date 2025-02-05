/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 11:07:13 by victor            #+#    #+#             */
/*   Updated: 2025/02/05 19:43:27 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*int	assign_single_var(char *arg, char **env)
{
	char	*pair[2];
	char	*equal_sign;
	int		i;

	i = 0;
	if (!check_key(arg))
		return (1);
	equal_sign = ft_strchr(arg, '=');
	if (!equal_sign) 
		return (0);

	char	*key;
	char	*value;
	key = ft_substr(arg, 0, equal_sign - arg);
	value = ft_strdup(equal_sign + 1);
	if (!key || !value)
	{
		free(key);
		free(value);
		return (1);
	}
	setenv(key, value, 1);
		free(key);
		free(value);

	if (equal_sign)
	{
		pair[0] = ft_substr(arg, 0, equal_sign - arg);
		pair[1] = ft_strdup(equal_sign + 1);
		if (!pair[0] || !pair[1])
		{
			free(pair[0]);
			free(pair[1]);
			return (1);
		}
		while (env[i])
		{
			if (!ft_strncmp(env[i], pair[0], ft_strlen(pair[0]))
				&& env[i][ft_strlen(pair[0])] == '=')
			{
				char *new_value = ft_strjoin(pair[0], "=");
				char *temp = ft_strjoin(new_value, pair[1]);
				free(new_value);
				if (env[i] && env[i] != arg)
					free(env[i]);
				env[i] = temp;
				free(pair[0]);
				free(pair[1]);
				return (0);
			}
			i++;
		}
		env[i] = ft_strjoin(pair[0], "=");
		char *new_value = ft_strjoin(env[i], pair[1]);
		free(env[i]);
		env[i] = new_value;
		env[i + 1] = NULL;
		free(pair[0]);
		free(pair[1]);
	}
	else
	{
		while (env[i])
		{
			if (ft_strcmp(env[i], arg) == 0)
				return (0);
			i++;
		}
		env[i] = ft_strdup(arg);
		env[i + 1] = NULL;
	}
	return (0);
}*/

int	validate_and_extract(char *arg, char **key, char **value)
{
	char	*equal_sign;

	equal_sign = ft_strchr(arg, '=');
	if (!equal_sign)
		return (1);
	*key = ft_substr(arg, 0, equal_sign - arg);
	*value = ft_strdup(equal_sign + 1);
	if (!(*key) || !(*value))
	{
		free(*key);
		free(*value);
		return (1);
	}
	remove_q(value);
	return (0);
}

int	update_existing_var(char **env, char *key, char *value)
{
	int		i;
	size_t	key_len;
	char	*new_value;
	char	*temp;

	key_len = ft_strlen(key);
	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], key, key_len) && env[i][key_len] == '=')
		{
			new_value = ft_strjoin(key, "=");
			temp = ft_strjoin(new_value, value);
			free(new_value);
			free(env[i]);
			env[i] = temp;
			return (1);
		}
		i++;
	}
	return (0);
}

void	add_new_var(char **env, char *key, char *value)
{
	int		i;
	char	*new_value;

	i = 0;
	while (env[i])
		i++;
	env[i] = ft_strjoin(key, "=");
	new_value = ft_strjoin(env[i], value);
	free(env[i]);
	env[i] = new_value;
	env[i + 1] = NULL;
}

int	assign_single_var(char *arg, char **env)
{
	char	*key;
	char	*value;

	if (!check_key(arg))
		return (1);
	if (!ft_strchr(arg, '='))
		return (0);
	if (validate_and_extract(arg, &key, &value))
		return (1);
	if (!update_existing_var(env, key, value))
	{
		add_new_var(env, key, value);
	}
	setenv(key, value, 1);
	free(key);
	free(value);
	return (0);
}

// Asigna las variables exportadas
int	assign_export_vars(char **av, char **env)
{
	int	i;
	int	result;

	i = 1;
	while (av[i])
	{
		result = assign_single_var(av[i], env);
		if (result)
			return (1);
		i++;
	}
	return (0);
}
