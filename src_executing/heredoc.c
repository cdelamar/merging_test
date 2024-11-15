/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdelamar <cdelamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 18:59:33 by cdelamar          #+#    #+#             */
/*   Updated: 2024/11/16 00:16:11 by cdelamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_heredoc(char *limit)
{
	char	*line;
	int		heredoc_fd;
	int		saved_stdin;

	saved_stdin = dup(STDIN_FILENO);
	if (saved_stdin < 0)
		return (-1);
	g_signal = 0;
	heredoc_signals();
	heredoc_fd = open("/tmp/heredoc_tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (heredoc_fd < 0)
	{
		reset_signals();
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdin);
		return (-1);
	}
	while (1)
	{
		if (g_signal == 1)
		{
			close(heredoc_fd);
			unlink("/tmp/heredoc_tmp");
			reset_signals();
			dup2(saved_stdin, STDIN_FILENO);
			close(saved_stdin);
			return (130);
		}
		line = readline("heredoc> ");
		if (!line)
		{
			write(STDOUT_FILENO, "\n", 1);
			close(heredoc_fd);
			reset_signals();
			dup2(saved_stdin, STDIN_FILENO);
			close(saved_stdin);
			return (0);
		}
		if (ft_strcmp(line, limit) == 0)
		{
			free(line);
			break ;
		}
		write(heredoc_fd, line, ft_strlen(line));
		write(heredoc_fd, "\n", 1);
		free(line);
	}
	close(heredoc_fd);
	reset_signals();
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdin);
	return (0);
}
