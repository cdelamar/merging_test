/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdelamar <cdelamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 13:13:15 by cdelamar          #+#    #+#             */
/*   Updated: 2024/09/06 00:07:09 by cdelamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/executing.h"

int ft_isnumber(char *str)
{
    if (!str || *str == '\0')
        return 0;

    if (*str == '-' || *str == '+')
        str++;

    while (*str)
    {
        if (!isdigit(*str))
            return 0;
        str++;
    }
    return 1;
}

int ft_exit(t_cmd *cmd)
{
    (void)cmd;
    int exit_code = 0;

    if (cmd->line_parsed[1] != NULL)
    {
        if (ft_isnumber(cmd->line_parsed[1]))
        {
            exit_code = ft_atoi(cmd->line_parsed[1]);
        }
        else
        {
            printf("exit: %s: numbers required\n", cmd->line_parsed[1]);
            exit_code = 255;
        }
    }

    // Free any allocated structures before exiting
    //free_structs(cmd); //double free

    // Return the exit code instead of calling exit()
    return exit_code;
}
