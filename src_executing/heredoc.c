/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdelamar <cdelamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 18:59:33 by cdelamar          #+#    #+#             */
/*   Updated: 2024/11/26 23:42:57 by cdelamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	init_heredoc(int *saved_stdin, int *heredoc_fd)
{
	*saved_stdin = dup(STDIN_FILENO);
	if (*saved_stdin < 0)
		return (-1);
	g_signal = 0;
	heredoc_signals();
	*heredoc_fd = open("/tmp/heredoc_tmp_minishell", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (*heredoc_fd < 0)
	{
		reset_signals();
		printf("sig 4\n");
		dup2(*saved_stdin, STDIN_FILENO);
		close(*saved_stdin);
		return (-1);
	}
	return (0);
}

static int	handle_signal_interrupt(int heredoc_fd, int saved_stdin)
{
	if (g_signal == 1)
	{
		close(heredoc_fd);
		unlink("/tmp/heredoc_tmp_minishell");
		reset_signals();
		printf("sig 3\n");

		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdin);
		return (130);
	}
	return (0);
}

static int	process_heredoc_line(char *line, char *limit, int heredoc_fd)
{
	if (!line)
	{
		write(STDOUT_FILENO, "\n", 1);
		//close(heredoc_fd);
		reset_signals();
		printf("sig 2\n");
		//close(saved_stdin);
		return (0);
	}
	if (ft_strcmp(line, limit) == 0)
	{
		free(line);
		return (1);
	}
	write(heredoc_fd, line, ft_strlen(line));
	write(heredoc_fd, "\n", 1);
	free(line);
	return (0);
}

int	ft_heredoc(char *limit)
{
	char	*line;
	int		heredoc_fd;
	int		saved_stdin;
	int		res;

	if (init_heredoc(&saved_stdin, &heredoc_fd) < 0)
		return (-1);
	while (1)
	{
		if (handle_signal_interrupt(heredoc_fd, saved_stdin) != 0)
			return (130);
		line = readline("heredoc> ");
		res = process_heredoc_line(line, limit, heredoc_fd);
		printf("coucou %d\n", res);
		if (res == 1)
			break ;
		else if (res == 0 && !line)
			return (0);
	}
	close(heredoc_fd);
	reset_signals();
	printf("sig 1\n");
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdin);
	return (0);
}
