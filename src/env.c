/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 11:46:45 by victor            #+#    #+#             */
/*   Updated: 2025/01/31 11:58:10 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int blt_env(char **env)
{
	int i = 0;

    // Recorremos el arreglo de entorno
    while (env[i])
    {
        // Imprimimos cada variable de entorno
        ft_printf("%s\n", env[i]);
        i++;
    }

    return 0;
}


int blt_unset(char **av, char **env)
{
    int i;
    int j;

    i = 1;
    if (!av[1])
    {
        ft_printf("unset: not enough arguments\n");
        return (1);
    }

    // Recorremos los argumentos para eliminar las variables de entorno
    while (av[i])
    {
        j = 0;
        // Buscamos la variable en el arreglo de entorno y la eliminamos si existe
        while (env[j])
        {
            if (ft_strcmp(av[i], env[j]) == 0)
            {
                // Desplazamos las variables en el arreglo para "eliminar" la actual
                while (env[j])
                {
                    env[j] = env[j + 1];
                    j++;
                }
                break;
            }
            j++;
        }
        i++;
    }

    return (0);
}

