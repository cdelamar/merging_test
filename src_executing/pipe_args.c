/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_args.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdelamar <cdelamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 23:46:53 by cdelamar          #+#    #+#             */
/*   Updated: 2024/11/15 23:49:33 by cdelamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	count_commands(char **final_tab)
{
	int	count;
	int	i;

	count = 1;
	i = 0;
	while (final_tab[i])
	{
		if (ft_strcmp(final_tab[i], "|") == 0)
			count++;
		i++;
	}
	return (count);
}

int	count_args(char **final_tab, int start)
{
	int	count;
	int	i;

	count = 0;
	i = start;
	while (final_tab[i] && ft_strcmp(final_tab[i], "|") != 0)
	{
		count++;
		i++;
	}
	return (count);
}

char	***split_commands(char **final_tab)
{
	int		num_commands;
	char	***commands;
	int		cmd_index;
	int		arg_index;
	int		i;

	num_commands = count_commands(final_tab);
	commands = malloc((num_commands + 1) * sizeof(char **));
	cmd_index = 0;
	arg_index = 0;
	i = 0;
	if (!commands)
		return (NULL);
	while (final_tab[i])
	{
		if (ft_strcmp(final_tab[i], "|") == 0)
		{
			commands[cmd_index][arg_index] = NULL;
			cmd_index++;
			arg_index = 0;
			i++;
			continue ;
		}
		if (arg_index == 0)
			commands[cmd_index] = malloc((count_args(final_tab, i) + 1)
				* sizeof(char *));
		commands[cmd_index][arg_index] = ft_strdup(final_tab[i]);
		arg_index++;
		i++;
	}
	commands[cmd_index][arg_index] = NULL;
	commands[cmd_index + 1] = NULL;
	return (commands);
}