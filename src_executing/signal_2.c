/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdelamar <cdelamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 16:53:57 by laubry            #+#    #+#             */
/*   Updated: 2024/11/15 22:58:17 by cdelamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	signals(void)
{
	setup_signal_handler(SIGINT, sigint_handler);
	setup_signal_handler(SIGQUIT, SIG_IGN);
	setup_signal_handler(SIGPIPE, SIG_IGN);
}

void	reset_signals(void)
{
	setup_signal_handler(SIGINT, SIG_DFL);
	setup_signal_handler(SIGQUIT, SIG_DFL);
}

void	print_token_list(t_token *head)
{
	t_token	*current;

	current = head;
	while (current != NULL)
	{
		printf("Index: %d\n", current->index);
		printf("Type: %s\n", get_token_type(current->type));
		printf("Content: %s\n", current->content);
		printf("\n");
		current = current->next;
	}
}
