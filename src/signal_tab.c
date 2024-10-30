/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_tab.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laubry <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 12:19:07 by laubry            #+#    #+#             */
/*   Updated: 2024/10/30 13:18:43 by laubry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void signal_main(char **envp, int boul)
{
	int			len;
	static char	**tab;

	if (boul == 0)
	{
		len = -1;
		rl_outstream = stderr;
		signal(SIGPIPE, SIG_IGN);
		signals();
		cmd = NULL;
		while (envp[++len]) {}
		tab = malloc(sizeof(char *) * (len + 1));
		cpy_tab(tab, envp);
	}
	else
	{
		cmd->heredoc_processed = FALSE;
		cmd->env = tab;
	}
}

