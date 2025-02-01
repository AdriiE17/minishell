/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 10:50:27 by victor            #+#    #+#             */
/*   Updated: 2025/01/31 11:02:43 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool is_special(const char *s)
{
    if (!ft_strcmp(s, ">>") ||
        !ft_strcmp(s, "<<") ||
        !ft_strcmp(s, ">") ||
        !ft_strcmp(s, "<") ||
        !ft_strcmp(s, "|"))
        return (true);
    return (false);
}

// You should receive **av moving its pointer to echo
//in case you have redirects or pipes
static bool	all_is_n(const char *s)
{
	int	i;

	i = 1;
	while (s[i] == 'n')
		i++;
	if (s[i] == '\0')
		return (true);
	else
		return (false);
}

static bool	check_n(char **av, int *j)
{
	bool	n;

	n = false;
	while (av[*j] && !strncmp(av[*j], "-n", 2))
	{
		if (*j == 0 && all_is_n(av[*j]))
		{
			n = true;
			(*j)++;
		}
		else if (all_is_n(av[*j]))
			(*j)++;
		else
			break ;
	}
	return (n);
}

int	blt_echo(char **av)
{
	bool	n;
	int		j;

	if (!av || !av[0]) // Si no hay argumentos, imprime un salto de línea y retorna
	{
		ft_printf("\n");
		return (0);
	}

	j = 0;
	n = check_n(av, &j); // No mover `av` aquí, ya llega desplazado en `ft_execute_echo`

	while (av[j])
	{
		ft_printf("%s", av[j]);
		if (av[j + 1])
			ft_printf(" ");
		j++;
	}
	if (!n)
		ft_printf("\n");
	return (0);
}

