#include "../includes/minishell.h"

int	check_all_commands_executable(char ***commands, char **env)
{
	int		i;
	char	*path;

	i = 0;
	while (commands[i] != NULL)
	{
		path = get_cmd_path(commands[i][0], env);
		if (path == NULL)
		{
			if (ft_strcmp(commands[i][0], "<") == 0 ||
				ft_strcmp(commands[i][0], ">") == 0 ||
				ft_strcmp(commands[i][0], "<<") == 0 ||
				ft_strcmp(commands[i][0], ">>") == 0)
				printf("redirections : syntax error\n");
			else
				fprintf(stderr, "%s: command not found\n", commands[i][0]);
			return (0);
		}
		free(path);
		i++;
	}
	return (1);
}

void	execute_builtin_child(t_cmd *cmd, char ***commands)
{
	if (pipe_builtin(cmd, cmd->path_command) == EXIT_SUCCESS)
		exit_with_clean(cmd, commands, EXIT_SUCCESS);
	else
		exit_with_clean(cmd, commands, EXIT_FAILURE);
}
