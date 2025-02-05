/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 10:50:27 by victor            #+#    #+#             */
/*   Updated: 2025/02/05 17:18:28 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_special(const char *s)
{
	if (!ft_strcmp(s, ">>") || !ft_strcmp(s, "<<")
		|| !ft_strcmp(s, ">") || !ft_strcmp(s, "<")
		|| !ft_strcmp(s, "|"))
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

int	exe_echo(char **av)
{
	bool	n;
	int		j;

	if (!av || !av[0])
	{
		ft_printf("\n");
		return (0);
	}
	j = 0;
	n = check_n(av, &j);
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
