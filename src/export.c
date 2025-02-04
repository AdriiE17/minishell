/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 11:07:13 by victor            #+#    #+#             */
/*   Updated: 2025/02/04 19:55:06 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Función para obtener el arreglo de entorno actualizado
char	**get_env_arr(char **env)
{
	int		i;
	int		j;
	int		length;
	char	**raw_env;

	i = 0;
	length = 0;
	while (env[length])
		length++;
	raw_env = malloc(sizeof(char *) * (length + 1));
	if (!raw_env)
		return (NULL);
	while (env[i])
	{
		raw_env[i] = ft_strdup(env[i]);
		if (!raw_env[i])
		{
			j = -1;
			while (++j < i)
				free(raw_env[j]);
			return (free(raw_env), NULL);
		}
		i++;
	}
	return (raw_env[i] = NULL, raw_env);
}

// Función para imprimir las variables exportadas
int	print_exported_vars(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		ft_printf("declare -x %s\n", env[i]);
		i++;
	}
	return (0);
}

// Comprobamos que la clave sea válida para una variable de entorno
bool	check_key(char *arg)
{
	int	i;

	i = 0;
	if (!ft_isalpha(arg[0]) && arg[0] != '_')
	{
		ft_putstr_fd("Not valid identifier\n", 2);
		return (false);
	}
	while (arg[i++])
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_' && arg[i] != '\0')
		{
			if (arg[i] != '=')
			{
				ft_putstr_fd("Not valid identifier\n", 2);
				return (false);
			}
			else
				break ;
		}
	}
	return (true);
}

int	assign_single_var(char *arg, char **env)
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

// Función principal de export
// Modificar blt_export para que env se actualice correctamente
void	free_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
		free(env[i++]);
	free(env);
}

int	blt_export(char **av, char ***env)
{
	int		result;
	char	**new_env;

	if (!av[1])
		return (print_exported_vars(*env));
	result = assign_export_vars(av, *env);
	if (result)
		return (result);
	new_env = get_env_arr(*env);
	if (!new_env)
		return (1);
	free_env(*env);
	*env = new_env;
	return (0);
}
