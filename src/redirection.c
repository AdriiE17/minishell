/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vberdugo <vberdugo@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 12:07:58 by vberdugo          #+#    #+#             */
/*   Updated: 2024/12/05 14:44:39 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int expand_exit_status(char *expanded, int i, int exit_status)
{
    char *exit_str;
    int j;

    exit_str = ft_itoa(exit_status);
    j = 0;
    while (exit_str[j])
        expanded[i++] = exit_str[j++];
    free(exit_str);
    return (i);
}

static int expand_env_var(char *expanded, char **ptr, int i)
{
    char *env_var;

    env_var = getenv(*ptr);
    if (env_var)
    {
        while (*env_var)
            expanded[i++] = *env_var++;
        while (**ptr && **ptr != ' ' && **ptr != '$')
            (*ptr)++;
    }
    else
    {
        (*ptr)++;
    }
    return (i);
}

void strip_single_quotes(char *str)
{
    int i = 0, j = 0;

    while (str[i])
    {
        if (str[i] == '\'' && (i == 0 || str[i - 1] != '\\'))
        {
            i++;
            continue;
        }
        str[j++] = str[i++];
    }
    str[j] = '\0';
}

void strip_double_quotes(char *str)
{
    int i = 0, j = 0;
    int in_dollar = 0;

    while (str[i])
    {
        if (str[i] == '"' && (i == 0 || str[i - 1] != '\\'))
        {
            i++;
            continue;
        }

        if (str[i] == '$' && !in_dollar)
        {
            in_dollar = 1;
        }
        else if (str[i] != '$' && in_dollar)
        {
            in_dollar = 0;
        }

        if (in_dollar || (str[i] != '$'))
        {
            str[j++] = str[i++];
        }
        else
        {
            i++;
        }
    }

    str[j] = '\0';
}

void strip_quotes(char *str)
{
    int len;
    len = ft_strlen(str);
    if ((str[0] == '\'' || str[0] == '"') && str[len - 1] == str[0])
    {
        ft_memmove(str, str + 1, len - 2);
        str[len - 2] = '\0';
    }
}

char *exp_env_vars(char *input, int exit_status)
{
    char *expanded;
    char *ptr;
    int i;
    int cap = 1024;
    expanded = malloc(cap);
    if (!expanded)
        return (NULL);
    ptr = input;
    i = 0;
    int in_single_quote = 0;
    int in_double_quote = 0;

    while (*ptr)
    {
        if (*ptr == '\'' && !in_double_quote)
        {
            in_single_quote = !in_single_quote;
            ptr++;
            continue;
        }
        else if (*ptr == '"' && !in_single_quote)
        {
            in_double_quote = !in_double_quote;
            ptr++;
            continue;
        }
        else if (*ptr == '$' && !in_single_quote)
        {
            ptr++;
            if (*ptr == '?')
            {
                ptr++;
                i = expand_exit_status(expanded, i, exit_status);
            }
            else
                i = expand_env_var(expanded, &ptr, i);
        }
        else
        {
            if (i >= cap - 1)
            {
                cap *= 2;
                expanded = ft_realloc(expanded, cap, cap * 2);
                if (!expanded)
                    return (NULL);
            }
            expanded[i++] = *ptr++;
        }
    }
    expanded[i] = '\0';
    return (expanded);
}



void handle_redirections(char **args, int *exit_status)
{
    int fd;
    int i = 0;
    int append;
    int in_single_quote = 0;
    int in_double_quote = 0;
    char *line;
    int pipefds[2];

    while (args[i])
    {
        if (args[i][0] == '\'' && !in_double_quote)
        {
            in_single_quote = !in_single_quote;
            i++;
            continue;
        }
        else if (args[i][0] == '"' && !in_single_quote)
        {
            in_double_quote = !in_double_quote;
            i++;
            continue;
        }
        if (!in_single_quote && !in_double_quote)
        {
            if (ft_strcmp(args[i], ">") == 0 || ft_strcmp(args[i], ">>") == 0)
            {
                if (i > 0 && args[i - 1][ft_strlen(args[i - 1]) - 1] == ' ')
                {
                    if (args[i + 1] && args[i + 1][0] == ' ')
                    {
                        append = ft_strcmp(args[i], ">>") == 0;
                        fd = open(args[i + 1] + 1, O_WRONLY | O_CREAT | (append ? O_APPEND : O_TRUNC), 0644);
                        if (fd == -1)
                        {
                            perror(args[i + 1]);
                            *exit_status = 1;
                            return;
                        }
                        dup2(fd, STDOUT_FILENO);
                        close(fd);
                        args[i] = NULL;
                    }
                    else
                    {
                        *exit_status = 1;
                        return;
                    }
                }
                else
                {
                    *exit_status = 1;
                    return;
                }
            }
            else if (ft_strcmp(args[i], "<") == 0)
            {
                fd = open(args[i + 1], O_RDONLY);
                if (fd == -1)
                {
                    perror(args[i + 1]);
                    *exit_status = 1;
                    return;
                }
                dup2(fd, STDIN_FILENO);
                close(fd);
                args[i] = NULL;
            }
            else if (ft_strcmp(args[i], "<<") == 0)
            {
                char *delimiter = args[i + 1];
                if (pipe(pipefds) == -1)
                {
                    perror("pipe");
                    *exit_status = 1;
                    return;
                }
                if (fork() == 0)
                {
                    close(pipefds[0]);
                    while ((line = readline("> ")) != NULL)
                    {
                        if (ft_strcmp(line, delimiter) == 0)
                            break;
                        write(pipefds[1], line, ft_strlen(line));
                        write(pipefds[1], "\n", 1);
                        free(line);
                    }
                    close(pipefds[1]);
                    exit(0);
                }
                close(pipefds[1]);
                dup2(pipefds[0], STDIN_FILENO);
                close(pipefds[0]);
                args[i] = NULL;
            }
        }
        i++;
    }
}

