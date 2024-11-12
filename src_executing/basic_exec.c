/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   basic_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdelamar <cdelamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 15:24:07 by cdelamar          #+#    #+#             */
/*   Updated: 2024/11/09 14:18:54 by laubry           ###   ########.fr       */
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
            g_signal = 2;
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



int	basic_parent_process(pid_t pid)
{
	int	status;

	if (waitpid(pid, &status, 0) == -1)
	{
		printf("waitpid -1\n");
		g_signal = 1;
		return (EXIT_FAILURE);
	}
	if (WIFEXITED(status))
		g_signal = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		g_signal = 128 + WTERMSIG(status);
	//if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_FAILURE)
	//	return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int basic_child_process(t_cmd *cmd)
{
    char    **split_line;
    char    *command;

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
    {
		close(cmd->fd[1]);
        execve(command, split_line, cmd->env);
    }
    else
    {
        printf("%s: command not found\n", split_line[0]);
		token_lstclear(&cmd->tokens, free);
        g_signal = 127;
    }

    // Écrit g_signal dans le pipe avant la sortie de l'enfant
    if (write(cmd->fd[1], (const void *)&g_signal, sizeof(g_signal)) == -1)
        perror("write error");
    close(cmd->fd[1]); // Ferme l'extrémité du pipe
    free(command);
    return (EXIT_FAILURE);
}

int basic_execute(t_cmd *cmd)
{
    int     exit_code;
    int     signal_value;

	if(ft_strcmp(cmd->final_tab[0], "$?") == 0)
	{
		printf("%d: command not found\n", g_signal);
		g_signal = 127;
		return (g_signal);
	}
    // Initialise le pipe pour transmettre la valeur de g_signal
    if (pipe(cmd->fd) == -1)
    {
        perror("pipe error");
        return (EXIT_FAILURE);
    }

    exit_code = ft_path_split(cmd);


    if (is_builtin(cmd->final_tab[0]) == true)
    {
        close(cmd->fd[0]);
        close(cmd->fd[1]);
        return (ft_builtin(cmd));
    }


    if (exit_code != EXIT_SUCCESS)
    {
        printf("je sais meme pas si on passe ici au moins une fois: %s\n", cmd->final_line);
        g_signal = 127;
		close(cmd->fd[0]);
        close(cmd->fd[1]);
        return (exit_code);
    }

    cmd->pid1 = fork();
    if (cmd->pid1 < 0)
    {
        perror("Fork error");
        close(cmd->fd[0]);
        close(cmd->fd[1]);
        return (EXIT_FAILURE);
    }
    else if (cmd->pid1 == 0)
    {
        close(cmd->fd[0]);
        exit_code = basic_child_process(cmd);
	    free(cmd->final_line);
        if (cmd->final_tab)
			ft_freetab(cmd->final_tab);
        ft_freetab(cmd->env);
        ft_freetab(cmd->path_split);
        token_lstclear(&cmd->tokens, free);
        free(cmd);
        exit(exit_code);
    }
    else
    {
        close(cmd->fd[1]);
        exit_code = basic_parent_process(cmd->pid1);
        if (read(cmd->fd[0], &signal_value, sizeof(signal_value)) > 0)
            g_signal = signal_value;
        close(cmd->fd[0]);

        return (exit_code);
    }
}
