#include "../includes/minishell.h"
/*
void	shift_left(char **split_line, int start_index)
{
	int	i;

	i = start_index;
	while (split_line[i + 2])
	{
		split_line[i] = split_line[i + 2];
		i++;
	}
	split_line[i] = NULL;
	split_line[i + 1] = NULL;
}

int	is_input_redirect(char *token)
{
	return (ft_strcmp(token, "<") == 0);
}

int	is_output_redirect(char *token)
{
	return (ft_strcmp(token, ">") == 0);
}

int	is_append_redirect(char *token)
{
	return (ft_strcmp(token, ">>") == 0);
}

int	is_heredoc_redirect(char *token, int status)
{
	return (ft_strcmp(token, "<<") == 0 && status == HEREDOC_ON);
}

int	handle_input_redirect(char **split_line, int i)
{
	if (ft_input_redirect(split_line, i) != EXIT_SUCCESS)
		return (-1);
	free(split_line[i]);
	free(split_line[i + 1]);
	shift_left(split_line, i);
	return (i);
}

int	handle_output_redirect(char **split_line, int i)
{
	if (ft_output_redirect(split_line, i, 0) != EXIT_SUCCESS)
		return (-1);
	free(split_line[i]);
	free(split_line[i + 1]);
	shift_left(split_line, i);
	return (i);
}

int	handle_append_redirect(char **split_line, int i)
{
	if (ft_output_redirect(split_line, i, 1) != EXIT_SUCCESS)
		return (-1);
	free(split_line[i]);
	free(split_line[i + 1]);
	shift_left(split_line, i);
	return (i);
}

int	handle_heredoc_redirect(char **split_line, int *i, t_cmd *cmd)
{
	if (split_line[*i + 1] == NULL)
	{
		printf("bash: syntax error near unexpected token `newline`\n");
		return (-1);
	}
	if (ft_heredoc_redirect(split_line[*i + 1]) < 0)
	{
		printf("ft_heredoc return -1\n");
		return (-1);
	}
	cmd->fd_in = open("/tmp/heredoc_tmp", O_RDONLY);
	if (cmd->fd_in < 0)
	{
		perror("Error opening heredoc temp file");
		return (-1);
	}
	dup2(cmd->fd_in, STDIN_FILENO);
	close(cmd->fd_in);
	free(split_line[*i]);
	free(split_line[*i + 1]);
	shift_left(split_line, *i);
	return (EXIT_SUCCESS);
}

int	handle_redirections(char **split_line, int status, t_cmd *cmd)
{
	int	i;

	i = 0;
	while (split_line[i])
	{
		if (is_output_redirect(split_line[i]))
			i = handle_output_redirect(split_line, i);
		else if (is_append_redirect(split_line[i]))
			i = handle_append_redirect(split_line, i);
		else if (is_input_redirect(split_line[i]))
			i = handle_input_redirect(split_line, i);
		else if (is_heredoc_redirect(split_line[i], status))
		{
			if (handle_heredoc_redirect(split_line, &i, cmd) != EXIT_SUCCESS)
				return (-1);
		}
		else
			i++;
	}
	return (EXIT_SUCCESS);
}*/
