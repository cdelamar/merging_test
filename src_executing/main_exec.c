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
/*
void shell_exec_loop(char **envp)
{
	char	*line;
	t_cmd	*cmd = NULL;
	int		len = -1;

	while (envp[++len]) {}
	char **tab = malloc(sizeof(char *) * (len + 1));
	cpy_tab(tab, envp);
	while (1)
	{
		if (malloc_structs(&cmd) != 0)
		{
			ft_putendl_fd(MALLOC_FAILURE, 2);
			return ;
		}
		cmd->heredoc_processed = FALSE;
		cmd->env = tab;
		line = readline("$ ");
		process_input(cmd);
		free(line); // LEAK
		tab = cmd->env;
	}
}*/
/*
int main(int argc, char **argv, char **envp)
{
	// g_signal = 0;
	(void)argv;

	if (argc != 1)
	{
		printf("invalid argument.\n");
		return (0);
	}
	rl_outstream = stderr;

	signals();

	shell_exec_loop(envp);
	return 0;
}*/

