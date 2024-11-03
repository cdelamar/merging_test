/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdelamar <cdelamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 19:01:29 by cdelamar          #+#    #+#             */
/*   Updated: 2024/10/22 16:48:13 by laubry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

extern sig_atomic_t	g_signal;

int	ft_output_redirect(char **split_line, int i, int append)
{
	int	fd;
	int	flags;

	if (ft_strcmp(split_line[0], ">") == 0
		|| ft_strcmp(split_line[0], ">>") == 0)
		return (EXIT_FAILURE);
	if (!split_line[i + 1])
	{
		printf("Syntax error: expected file after redirection\n");
		return (EXIT_FAILURE);
	}
	if (append)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	fd = open(split_line[i + 1], flags, 0644);
	if (fd < 0)
		return (EXIT_FAILURE);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	split_line[i] = NULL;
	return (EXIT_SUCCESS);
}

void shift_left(char **split_line, int start_index) {
    int i = start_index;

    // Shift all elements to the left by two positions
    while (split_line[i + 2]) {
        split_line[i] = split_line[i + 2];
        i++;
    }
    split_line[i] = NULL; // Terminate the new list
    split_line[i + 1] = NULL;
}


int	ft_input_redirect(char **split_line, int i)
{
	int	fd;

	if (!split_line[i + 1])
	{
		printf("Syntax error: expected file after redirection\n");
		return (EXIT_FAILURE);
	}
	fd = open_file(split_line[i + 1], O_RDONLY, 0);
	if (fd < 0)
		return (EXIT_FAILURE);
	dup2(fd, STDIN_FILENO);
	close(fd);
	split_line[i] = NULL;
	return (EXIT_SUCCESS);
}

int	ft_heredoc_redirect(char *delim)
{
	if (!delim)
		return (EXIT_FAILURE);
	if (ft_heredoc(delim) != 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int handle_redirections(char **split_line, int status, t_cmd *cmd) {
    int i = 0;

    while (split_line[i]) {
        if (ft_strcmp(split_line[i], ">") == 0)
		{
            if (ft_output_redirect(split_line, i, 0) != EXIT_SUCCESS)
                return (EXIT_FAILURE);
            free(split_line[i]);
            free(split_line[i + 1]);
            shift_left(split_line, i);
        }

        else if (ft_strcmp(split_line[i], ">>") == 0)
		{
            if (ft_output_redirect(split_line, i, 1) != EXIT_SUCCESS)
                return (EXIT_FAILURE);
            free(split_line[i]);
            free(split_line[i + 1]);
            shift_left(split_line, i);
        }
        else if (ft_strcmp(split_line[i], "<") == 0)
		{
            if (ft_input_redirect(split_line, i) != EXIT_SUCCESS)
                return (EXIT_FAILURE);
            free(split_line[i]);
            free(split_line[i + 1]);
            shift_left(split_line, i);
        }
        else if (ft_strcmp(split_line[i], "<<") == 0 && status == HEREDOC_ON)
		{
            if (split_line[i + 1] == NULL)
			{
                printf("bash: syntax error near unexpected token `newline`\n");
                return (-1);
            }
            if (ft_heredoc_redirect(split_line[i + 1]) < 0) {
                printf("ft_heredoc return -1\n");
                return (-1);
            }
            cmd->fd_in = open("/tmp/heredoc_tmp", O_RDONLY);
            if (cmd->fd_in < 0) {
                perror("Error opening heredoc temp file");
                return (-1);
            }
            dup2(cmd->fd_in, STDIN_FILENO);
            close(cmd->fd_in);
            free(split_line[i]);
            free(split_line[i + 1]);
            shift_left(split_line, i);
        }
        else
            i++;
    }
    return (EXIT_SUCCESS);
}


/*
int	handle_redirections(char **split_line, int status, t_cmd *cmd)
{
	int	i;

	i = 0;
	while (split_line[i])
	{
		if (ft_strcmp(split_line[i], ">") == 0)
		{
			if (ft_output_redirect(split_line, i, 0) != EXIT_SUCCESS)
				return (EXIT_FAILURE);
		}
		else if (ft_strcmp(split_line[i], ">>") == 0)
		{
			if (ft_output_redirect(split_line, i, 1) != EXIT_SUCCESS)
				return (EXIT_FAILURE);
		}
		else if (ft_strcmp(split_line[i], "<") == 0)
		{
			if (ft_input_redirect(split_line, i) != EXIT_SUCCESS)
				return (EXIT_FAILURE);
		}
		else if (ft_strcmp(split_line[i], "<<") == 0 && status == HEREDOC_ON)
		{
			if (split_line[i + 1] == NULL)
			{
				printf("bash: syntax error near unexpected token `newline`\n");
				return (-1);
			}
			if ((ft_heredoc(split_line[i + 1]) < 0))
			{
				printf("ft_heredoc return -1\n");
				return (-1);
			}
			cmd->fd_in = open("/tmp/heredoc_tmp", O_RDONLY);
			if (cmd->fd_in < 0)
			{
				perror("perror ligne 125 de redirection.c");
				return (-1);
			}
			dup2(cmd->fd_in, STDIN_FILENO);
			close(cmd->fd_in);
			free(split_line[i]);
			free(split_line[i + 1]);
			while (split_line[i + 2])
			{
				split_line[i] = split_line[i + 2];
				i++;
			}
			split_line[i] = NULL;
			i = 0;
		}
		else
			i++;
	}
	return (EXIT_SUCCESS);
}*/