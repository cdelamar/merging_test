/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdelamar <cdelamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 17:05:29 by cdelamar          #+#    #+#             */
/*   Updated: 2024/11/09 14:02:07 by laubry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	skip_x1f(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '\x1F')
			i++;
		else
		{
			ft_putchar_fd(line[i], 1);
			i++;
		}
	}
}

static void	echo_output(char **split_line, int i)
{
	int	start;

	start = i;
	while (split_line[start])
	{
		if (ft_strchr(split_line[start], '\x1F'))
		{
			skip_x1f(split_line[start]);
			start++;
		}
		else
		{
			ft_putstr_fd(split_line[start], 1);
			start++;
			if (split_line[start])
				ft_putchar_fd(' ', 1);
		}
	}
	return ;
}

int	ft_echo(char **split_line)
{
	int		i;
	int		j;
	bool	newline;

	i = 1;
	newline = true;

	while (split_line[i] && split_line[i][0] == '-')
	{
		j = 1;
		while (split_line[i][j] == 'n')
			j++;
		if (split_line[i][j] != '\0')
			break ;
		newline = false;
		i++;
	}

	if (split_line[i] == NULL)
	{
		if (newline)
			ft_putchar_fd('\n', 1);
		return (EXIT_SUCCESS);
	}

//	if (ft_strcmp(split_line[i], "$?") == 0)
//	{
//		printf("%d\n", g_signal);
//		g_signal = 0;
//	}
	else
	{
		echo_output(split_line, i);
		if (newline)
			ft_putchar_fd('\n', 1);
	}
	return (EXIT_SUCCESS);
}
