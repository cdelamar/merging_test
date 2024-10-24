/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   basic_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdelamar <cdelamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 15:24:07 by cdelamar          #+#    #+#             */
/*   Updated: 2024/10/22 15:53:16 by laubry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool	syntax_redirect(char *line)
{
	int		i;
	char	**split_line;

	i = 0;
	split_line = ft_split(line, ' ');
	while (split_line[i] != NULL)
	{
		if ((strcmp(split_line[i], ">") == 0 && split_line[i + 1] != NULL
				&& strcmp(split_line[i + 1], ">") == 0)
			|| (strcmp(split_line[i], "<") == 0 && split_line[i + 1] != NULL
				&& strcmp(split_line[i + 1], "<") == 0))
		{
			printf("Error: consecutive redirections ('%s %s') not allowed.\n",
				split_line[i], split_line[i + 1]);
			ft_freetab(split_line);
			return (false);
		}
		i++;
	}
	ft_freetab(split_line);
	return (true);
}

int	ft_path_split(t_cmd *cmd)
{
	ft_path(cmd);
	if (!cmd->path)
	{
		printf("SET COMMAND PATH command not found\n");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	basic_child_process(t_cmd *cmd)
{
	char	**split_line;
	char	*command;

	split_line = NULL;
	split_line = cmd->final_tab;
	if (!split_line)
		return (EXIT_FAILURE);
	if (handle_redirections(split_line, HEREDOC_ON, cmd) != 0)
	{
		ft_freetab(split_line);
		return (EXIT_FAILURE);
	}
	command = cmd_finder(split_line, cmd);
	if (command)
		execve(command, split_line, cmd->env);
	print_tab(split_line);
	printf("%s : command not found\n", split_line[0]);
	free(command);
	close(cmd->fd_in);
	return (EXIT_FAILURE);
}

int	basic_parent_process(pid_t pid)
{
	int	status;

	if (waitpid(pid, &status, 0) == -1)
	{
		printf("waitpid -1\n");
		return (EXIT_FAILURE);
	}
	if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	basic_execute(t_cmd *cmd)
{
	int		exit_code;
	char	**split_line;

	split_line = NULL;
	exit_code = ft_path_split(cmd);
	if (exit_code != EXIT_SUCCESS)
	{
		printf("Command not found: %s\n", cmd->final_line);
		if (split_line)
			ft_freetab(split_line);
		return (exit_code);
	}
	cmd->pid1 = fork();
	if (cmd->pid1 < 0)
	{
		printf("Fork error\n");
		ft_freetab(split_line);
		return (EXIT_FAILURE);
	}
	else if (cmd->pid1 == 0)
	{
		exit_code = basic_child_process(cmd);
		free(cmd->final_line);
		ft_freetab(cmd->final_tab);
		ft_freetab(cmd->env);
		ft_freetab(cmd->path_split);
		free(cmd);
		exit(exit_code);
	}
	else
	{
		exit_code = basic_parent_process(cmd->pid1);
		return (exit_code);
	}
}
