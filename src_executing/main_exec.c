/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdelamar <cdelamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 18:06:47 by cdelamar          #+#    #+#             */
/*   Updated: 2024/10/22 16:10:27 by laubry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


bool synthax_manager(char **split_line)
{
	int i;

	i = 0;
	while(split_line[i])
		i++;

	if (ft_strcmp(split_line[i - 1], "|") == 0 || split_line[0][0] == '|' )
	{
		printf("synthax error near \"|\"\n");
		return (true);
	}


	if (ft_strcmp(split_line[0], "<") == 0 || ft_strcmp(split_line[0], ">") == 0
		|| ft_strcmp(split_line[0], "<<") == 0 || ft_strcmp(split_line[0], ">>") == 0)
	{
		printf("redirection have to be implemented between a command and an argument\n");
		return (true);
	}
	return (false);
}

void	process_input(t_cmd *cmd)
{
	if (synthax_manager(cmd->final_tab))
		return;
	execute(cmd);
}

char	**cpy_tab(char **dest, char **src)
{
	int	len;

	len = -1;
	while (src[++len])
		dest[len] = ft_strdup(src[len]);
	dest[len] = NULL;
	return (dest);
}
