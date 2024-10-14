/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdelamar <cdelamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 18:06:47 by cdelamar          #+#    #+#             */
/*   Updated: 2024/10/03 02:14:07 by cdelamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void process_input(t_cmd *cmd)
{
	if (cmd->final_line == NULL)
	{
		//printf("CTRL + D from shell_loop\n");
		ft_freetab(cmd->env);
		free_structs(cmd);
		exit(0); // Handle exit on EOF (CTRL + D)
	}

	if (space_only(cmd->final_line) == true)
	{
		//free_structs(cmd);
		return;
	}

	if (cmd->final_line)
		add_history(cmd->final_line);

	//printf ("execute = %d\n", execute(cmd));
	execute(cmd); // execute retourne un int mais je men sert pas
}

char **cpy_tab(char **dest, char **src)
{
	int len = -1;
	while (src[++len])
		dest[len] = ft_strdup(src[len]);
	dest[len] = NULL;
	return dest;
}


