/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laubry <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 12:38:11 by laubry            #+#    #+#             */
/*   Updated: 2024/10/30 13:09:55 by laubry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile int g_signal = 0;

int	signal_0(char *line)
{
 	if (space_only(line) == true)
	{
		printf("erreur only space\n");
		g_signal = 0;
		return (1);
	}
	return (0);
}

int	signal_2(char **split_line, char *line)
{
	if (ft_strcmp(split_line[0], "|") == 0 || !check_pipe(split_line))
	{
		printf("erreur synthaxe symbole\n");
		free_split_line(split_line);
		free(line);
		g_signal = 2;
		return (1);
	}
	return (0);
}

int	signal_127(char **split_line, char *line)
{
	if (solo_quote(split_line) || badchar(split_line))
	{
		free_split_line(split_line);
		free(line);
		g_signal = 127;
		printf("synthax error : unmatching quote\n");
		return (1);
	}
	return (0);
}

int	error_main(char *line, char **split_line, t_token **token_list)
{
	if (line == NULL)
	{
		ft_freetab(cmd->env);
		free_cmd(cmd);
		g_signal = 0;
		return (1);
	}
	add_history(line);
	if (signal_0(line))
		return (1);
	if (signal_2(split_line, line))
		return (1);
	if (signal_127(split_line, line))
		return (1);
	if (!make_token(split_line, token_list))
	{
		free_split_line(split_line);
		free(line);
		g_signal = 0;
		return (1);
	}
	cmd->tokens = copy_token_list(*token_list);
	return (0);
}
