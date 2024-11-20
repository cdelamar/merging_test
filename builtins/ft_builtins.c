#include "../includes/minishell.h"

int	redirect_manager(char **split_line, int saved_stdin,
		int saved_stdout, t_cmd *cmd)
{
	if (handle_redirections(split_line, 0, cmd) == EXIT_FAILURE)
	{
		restore_fd(saved_stdin, saved_stdout);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	backup_manager(char **split_line, int *saved_stdin,
		int *saved_stdout, t_cmd *cmd)
{
	(void)split_line;
	if (backup_fd(saved_stdin, saved_stdout) < 0)
		return (EXIT_FAILURE);
	if (redirect_manager(split_line, *saved_stdin, *saved_stdout, cmd)
		== EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

bool	is_builtin(char *command)
{
	if (!command)
		return (false);
	return (ft_strcmp(command, "unset") == 0
		|| ft_strcmp(command, "echo") == 0
		|| ft_strcmp(command, "cd") == 0
		|| ft_strcmp(command, "export") == 0
		|| ft_strcmp(command, "env") == 0
		|| ft_strcmp(command, "pwd") == 0
		|| ft_strcmp(command, "exit") == 0);
}

int	pipe_builtin(t_cmd *cmd, char **command)
{
	int		saved_in;
	int		saved_out;
	int		ret;

	if (backup_manager(command, &saved_in, &saved_out, cmd) == EXIT_SUCCESS)
		ret = builtin_commands(command, cmd, saved_in, saved_out);
	else
		ret = (EXIT_FAILURE);
	return (ret);
}

int	ft_builtin(t_cmd *cmd)
{
	char	**split_line;
	int		saved_in;
	int		saved_out;
	int		ret;

	split_line = cmd->final_tab;
	if (!split_line)
		return (EXIT_FAILURE);
	if (backup_manager(split_line, &saved_in, &saved_out, cmd) == EXIT_SUCCESS)
		ret = builtin_commands(split_line, cmd, saved_in, saved_out);
	else
		ret = (EXIT_FAILURE);
	return (ret);
}
