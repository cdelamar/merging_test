/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdelamar <cdelamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 15:23:45 by cdelamar          #+#    #+#             */
/*   Updated: 2024/10/03 02:21:45 by cdelamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/minishell.h"

static void setup_child_pipes(t_cmd *cmd, int *fd, int i)
{
	dup2(cmd->fd_in, 0); // Get input from the previous command
	if (cmd->path_command[i + 1])
		dup2(fd[1], 1); // Output to the next command
	close_fds(fd);
	close(cmd->fd_in);
}

static int child_process(t_cmd *cmd, int *fd, int i)
{
	setup_child_pipes(cmd, fd, i);

	if (basic_execute(cmd, PIPE_EXEC, i) == EXIT_FAILURE) // EXIT SUCCES OR EXIT FAILURE
	{
		ft_freetab(cmd->path_command);
		ft_freetab(cmd->path_split);
		if(cmd->env)
			ft_freetab(cmd->env);
		free(cmd);
		exit(EXIT_FAILURE); // MAJOR success OR failure
	}
	ft_freetab(cmd->path_command);
	ft_freetab(cmd->path_split);
	if(cmd->env)
		ft_freetab(cmd->env);
	if (cmd->final_line)
		free(cmd->final_line);
	free(cmd);
	//printf("ca leak ici nan\n");
	//token_lstclear(&token_list, free);
	// ft_freetab(cmd->env);
	// free_cmd(cmd);
	exit(EXIT_SUCCESS);
}

static void parent_process(t_cmd *cmd, int *fd)
{
	close(fd[1]);
	if (cmd->fd_in)
		close(cmd->fd_in);
	cmd->fd_in = fd[0]; // save input pour la prochaine commande
}

static int create_and_fork(t_cmd *cmd, int *fd)
{
	if (pipe(fd) < 0)
		handle_error("exec.c line 49", cmd, fd);
	cmd->pid1 = fork();
	if (cmd->pid1 < 0)
		handle_error("exec.c line 57", cmd, fd);
	return cmd->pid1;
}

int pipe_execute(t_cmd *cmd)
{
	int i = 0;
	pid_t last_pid = -1;

	ft_path_command(cmd); // Split command by pipe '|'

	while (cmd->path_command[i])
	{
		//printf ("path_command %d = %s\n", i, cmd->path_command[i]);
		if (create_and_fork(cmd, cmd->fd) == 0)
		{
			// Child process
			child_process(cmd, cmd->fd, i);
		}
		else
		{
			last_pid = cmd->pid1; // save le dernier child pid
			parent_process(cmd, cmd->fd);
		}
		i++;
	}

	// Wait for last comsmand in the pipeline
	int status;
	if (last_pid > 0)
		waitpid(last_pid, &status, 0);

	// Wait for any remaining zombie child processes
	while (waitpid(-1, NULL, WNOHANG) > 0);

	close(cmd->fd_in); // Close the last file descriptor

	if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS)
		return (EXIT_SUCCESS);
	else
		return (EXIT_FAILURE);
}


