/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdelamar <cdelamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 18:59:33 by cdelamar          #+#    #+#             */
/*   Updated: 2024/10/22 16:09:02 by laubry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

extern sig_atomic_t	g_signal;

int	ft_heredoc(char *limit)
{
	char	*line;
	int		heredoc_fd;
	int		saved_stdin;

	// Backup de linput de base (fd 0)
	saved_stdin = dup(STDIN_FILENO);
	if (saved_stdin < 0)
		return (-1);

	g_signal = 0;  // reset g_signal, obligatoire ?
	heredoc_signals();

	heredoc_fd = open("/tmp/heredoc_tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (heredoc_fd < 0)
	{
		reset_signals();  // Reset signals before return stuff
		dup2(saved_stdin, STDIN_FILENO);  // Restore original stdin (AVOID BAD FD)
		close(saved_stdin);
		return (-1);
	}

	while (1)
	{
		if (g_signal == 1)  // for ctrl C
		{
			// Cleanup + exit for ctrl + C
			close(heredoc_fd);
			unlink("/tmp/heredoc_tmp");  // unlinke the file buffer
			reset_signals();
			dup2(saved_stdin, STDIN_FILENO);  // restore original stdard input
			close(saved_stdin);
			return (130);  // Return 130 (exit status for Ctrl + C)
		}

		line = readline("heredoc> ");
		if (!line)  // Handle EOF (Ctrl + D)
		{
			write(STDOUT_FILENO, "\n", 1);  // saut a la ligne apres D
			close(heredoc_fd);
			reset_signals();  // important
			dup2(saved_stdin, STDIN_FILENO);  // meme sauce
			close(saved_stdin); // faut que je recheck je ferme quoi deja
			return (0);
		}

		// for EOF if cat << EOF, or anything else, delim though
		if (ft_strcmp(line, limit) == 0)
		{
			free(line);
			break;
		}

		// write() the input line to the heredoc file buffer
		write(heredoc_fd, line, ft_strlen(line));
		write(heredoc_fd, "\n", 1); // GNL discount
		free(line);
	}

	close(heredoc_fd);
	reset_signals();  // Restore default signal handling
	dup2(saved_stdin, STDIN_FILENO);  // Restore original stdi AVOID BAD FD
	close(saved_stdin);  // ferme le backup fd comme pourl es autres
	return (0);
}

