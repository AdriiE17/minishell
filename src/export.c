/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 11:07:13 by victor            #+#    #+#             */
/*   Updated: 2025/01/31 18:48:10 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

// Función para obtener el arreglo de entorno actualizado
char **get_env_arr(char **env)
{
    int i = 0;
    int length = 0;

    // Contamos el número de variables de entorno
    while (env[length])
        length++;

    // Asignamos memoria para el nuevo arreglo
    char **raw_env = malloc(sizeof(char *) * (length + 1));
    if (!raw_env)
        return (NULL);

    // Copiamos las variables de entorno al nuevo arreglo
    while (env[i])
    {
        raw_env[i] = ft_strdup(env[i]);
        if (!raw_env[i])
        {
            // Liberamos correctamente si ocurre un error
            for (int j = 0; j < i; j++)
                free(raw_env[j]);
            free(raw_env);
            return (NULL);
        }
        i++;
    }
    raw_env[i] = NULL;  // Finalizamos el arreglo con NULL
    return (raw_env);
}

// Función para imprimir las variables exportadas
int print_exported_vars(char **env)
{
    int i = 0;

    while (env[i])
    {
        ft_printf("declare -x %s\n", env[i]);
        i++;
    }
    return (0);
}

// Comprobamos que la clave sea válida para una variable de entorno
bool check_key(char *arg)
{
    int i = 0;
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
                break;
        }
    }
    return (true);
}

// Asigna una sola variable de entorno
int assign_single_var(char *arg, char **env)
{
    char *pair[2];
    char *equal_sign;
    int i = 0;

    if (!check_key(arg))  // Comprobamos que el nombre sea válido
        return (1);

    equal_sign = ft_strchr(arg, '=');
    if (equal_sign)
    {
        pair[0] = ft_substr(arg, 0, equal_sign - arg); // Nombre de la variable
        pair[1] = ft_strdup(equal_sign + 1); // Valor de la variable
        if (!pair[0] || !pair[1])
        {
            free(pair[0]);
            free(pair[1]);
            return (1); // Error en la asignación
        }

        while (env[i]) // Buscamos si la variable ya existe
        {
            if (ft_strncmp(env[i], pair[0], ft_strlen(pair[0])) == 0)
            {
                free(env[i]);  // Liberamos la antigua variable
                env[i] = ft_strjoin(pair[0], "=");
                env[i] = ft_strjoin(env[i], pair[1]);
                free(pair[0]);
                free(pair[1]);
                return (0);  // Variable actualizada correctamente
            }
            i++;
        }

        // Si no se encuentra la variable, asignarla al final del arreglo
        env[i] = ft_strjoin(pair[0], "=");
        env[i] = ft_strjoin(env[i], pair[1]);
        env[i + 1] = NULL;  // Asegurarse de que el arreglo termine en NULL
        free(pair[0]);
        free(pair[1]);
    }
    else
    {
        while (env[i])  // Buscamos si la variable ya existe
        {
            if (ft_strcmp(env[i], arg) == 0)
                return (0);  // Ya existe, no hacer nada
            i++;
        }

        // Si no se encuentra la variable, agregarla al final
        env[i] = ft_strdup(arg);
        env[i + 1] = NULL;  // Asegurarse de que el arreglo termine en NULL
    }
    return (0);
}

// Asigna las variables exportadas
int assign_export_vars(char **av, char **env)
{
    int i = 1; // Comenzamos en 1 para saltar el primer argumento (nombre del comando)
    int result;

    while (av[i])
    {
        result = assign_single_var(av[i], env);
        if (result)
            return (1);  // Si hay un error en alguna variable, retornamos 1
        i++;
    }
    return (0);
}

// Función principal de export
int blt_export(char **av, char **env)
{
    int result;

    if (!av[1])  // Si no hay argumentos, mostramos las variables exportadas
        return (print_exported_vars(env));

    // Si hay argumentos, los asignamos
    result = assign_export_vars(av, env);
    if (result)
        return (result);

    // Obtenemos el arreglo de entorno actualizado
    char **new_env = get_env_arr(env);
    if (!new_env)
        return (1);  // Error en la actualización del entorno

    // Actualizamos la variable global env
    env = new_env;
    return (0);
}

