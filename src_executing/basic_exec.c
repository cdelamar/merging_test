/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   basic_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdelamar <cdelamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 15:24:07 by cdelamar          #+#    #+#             */
/*   Updated: 2024/10/03 03:10:13 by cdelamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool syntax_redirect(char *line)
{
	int i = 0;
	char **split_line;

	split_line = ft_split(line, ' ');
	while (split_line[i] != NULL)
	{
		if ((strcmp(split_line[i], ">") == 0 && split_line[i + 1] != NULL && strcmp(split_line[i + 1], ">") == 0) ||
			(strcmp(split_line[i], "<") == 0 && split_line[i + 1] != NULL && strcmp(split_line[i + 1], "<") == 0))
		{
			printf("Error: consecutive redirections ('%s %s') are not allowed.\n", split_line[i], split_line[i + 1]);
			ft_freetab(split_line); //LEAK
			return (false);
		}
		i++;
	}
	ft_freetab(split_line); //LEAK
	return (true);
}

int ft_path_split(t_cmd *cmd)
{
	ft_path(cmd);
	if (!cmd->path)
	{
		printf("SET COMMAND PATH command not found\n");
		return (EXIT_FAILURE);
		//EXIT SUCCESS
	}
	return (EXIT_SUCCESS);
}

int basic_child_process(char **free_line, char *line, t_cmd *cmd)
{
	char **split_line;
	char *command;

	split_line = ft_split(line, ' ');
	//print_tab(split_line);
	if (!split_line)
		return (EXIT_FAILURE);

	if (handle_redirections(split_line, HEREDOC_ON, cmd) != 0)
	{
		ft_freetab(split_line);
		return (EXIT_FAILURE);
	}

	command = cmd_finder(split_line, cmd);
	if (command)
	{
		//printf("execve\n");
		execve(command, split_line, cmd->env); // exit
	}
	printf("%s : command not found\n", line);
	ft_freetab(split_line);
	ft_freetab(free_line);
	free_cmd(cmd);
	// token_lstclear(&token_list, free);
	exit (EXIT_FAILURE);
}

int basic_parent_process(pid_t pid) // TODO free cmd->path_split
{
	int status;

	if (waitpid(pid, &status, 0) == -1)
	{
		printf("waitpid -1\n");
		return EXIT_FAILURE;
	}
	if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_FAILURE)
		return EXIT_FAILURE;
	return EXIT_SUCCESS;
}

int basic_execute(char *line, t_cmd *cmd)
{
	int exit_code;
	char **split_line = NULL;

	split_line = ft_split(line, ' ');
	//print_tab(split_line);
	exit_code = ft_path_split(cmd); // Find command path

	if (exit_code != EXIT_SUCCESS)
	{
		printf("Command not found: %s\n", line);
		ft_freetab(split_line); // Free command split
		return exit_code;
	}

	cmd->pid1 = fork();
	if (cmd->pid1 < 0)
	{
		printf("Fork error\n");
		ft_freetab(split_line);
		return EXIT_FAILURE;
	}
	else if (cmd->pid1 == 0)
	{
		// Child process
		exit_code = basic_child_process(split_line, line, cmd);
		ft_freetab(split_line); // Free split in child process
		exit(exit_code);        // Avoid duplicate execution
	}
	else
	{
		// Parent process
		exit_code = basic_parent_process(cmd->pid1);
		ft_freetab(split_line); // Free split in parent process
		return exit_code;       // Return success or failure
	}
}
