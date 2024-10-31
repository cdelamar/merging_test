/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laubry <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 12:14:00 by laubry            #+#    #+#             */
/*   Updated: 2024/10/31 00:58:10 by laubry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//metre signal_main dans le makefile
#include "minishell.h"

volatile int g_signal = 0;

int main(int argc, char **argv, char **envp)
{
	int	error;
	char **split_line;
	char	*line;
	t_cmd	*cmd;
	t_token *token_list;

	cmd = NULL;
	error = 0;
	token_list = NULL;
	(void)argv;
	signal_main(envp, 0, cmd);
	if (argc > 1)
	{
		g_signal = 0;
		return (check_error(ERROR_ARGS));
	}
	while (1)
	{
		if (malloc_structs(&cmd) != 0)
		{
			ft_putendl_fd(MALLOC_FAILURE, 2);
			g_signal = 1;
			error = 1;	
		}
		signal_main(envp, 1, cmd);
		line = readline("$ ");
		split_line = ft_split_boosted(line);
		if (line == NULL)
		{
			ft_freetab(cmd->env);
			free_cmd(cmd);
			g_signal = 0;
			error = 1;
			break;
		}
		if (split_line == NULL)
			continue;
		error += error_main(line, split_line, &token_list, cmd);
		if (error <= 0)
		{	
			path_main(token_list, envp);
			cmd->final_tab = main_cat(&token_list);
			cmd->final_line = tab_to_str(cmd->final_tab);
			free(split_line);
			free(line);
			token_lstclear(&token_list, free);
			process_input(cmd);
			free(cmd->final_line);
			token_lstclear(&cmd->tokens, free);
			signal_main(envp, 2, cmd);
		}
	}
	return (0);
}
