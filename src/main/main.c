/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laubry <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 12:14:00 by laubry            #+#    #+#             */
/*   Updated: 2024/10/30 13:22:46 by laubry           ###   ########.fr       */
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
	t_token *token_list;

	error = 0;
	token_list = NULL;
	(void)argv;
	signal_main(envp, 0);
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
		line = readline("$ ");
		split_line = ft_split_boosted(line);
		signal_main(envp, 1);
		error += error_main(line, split_line, &token_list);
		if (error <= 0)
		{	
			path_main(token_list, envp);
			cmd->final_tab = main_cat(&token_list);
			cmd->final_line = tab_to_str(cmd->final_tab);
			free(split_line);
			free(line);
			token_lstclear(&token_list, free);
			process_input(cmd);
		}
	}
	return (0);
}
