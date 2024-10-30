/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laubry <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 16:53:57 by laubry            #+#    #+#             */
/*   Updated: 2024/10/22 16:54:48 by laubry           ###   ########.fr       */
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
