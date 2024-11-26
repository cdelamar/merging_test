#include "../includes/minishell.h"

bool is_redirection(char *token)
{
	if (!token)
		return (false);
	return (strcmp(token, ">") == 0
			|| strcmp(token, ">>") == 0
			|| strcmp(token, "<") == 0
			|| strcmp(token, "<<") == 0);
}

int	handle_redirections_with_front_check(char **split_line, t_cmd *cmd)
{
	if (!split_line[0])
		return (EXIT_SUCCESS);

	if (ft_strcmp(split_line[0], "<") == 0)
		return handle_input_redirection(split_line, 0);
	else if (ft_strcmp(split_line[0], "<<") == 0)
		return handle_heredoc_redirection(split_line, 0, cmd);
	else if (ft_strcmp(split_line[0], ">>") == 0)
		return handle_output_redirection(split_line, 0, 1);
	else if (ft_strcmp(split_line[0], ">") == 0)
		return handle_output_redirection(split_line, 0, 0);

	return (EXIT_SUCCESS);
}

int	preprocess_redirections(char **split_line, t_cmd *cmd)
{
	if (split_line[0] && (ft_strcmp(split_line[0], "<") == 0 || ft_strcmp(split_line[0], "<<") == 0))
	{
		if (handle_redirections_with_front_check(split_line, cmd) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}


int	validate_redirections(char **split_line)
{
	int	i;
	char *error_token;
	i = 0;
	while (split_line[i])
	{
		if ((ft_strcmp(split_line[i], "<") == 0 || ft_strcmp(split_line[i], "<<") == 0) &&
			(split_line[i + 1] == NULL || is_redirection(split_line[i + 1])))
		{
			if (split_line[i + 1] != NULL)
				error_token = split_line[i + 1];
			else
				error_token = "newline";
			printf("bash: syntax error near unexpected token `%s`\n", error_token);
			return (EXIT_FAILURE);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}


/*
int	validate_redirections(char **split_line)
{
	int	i;

	i = 0;
	while (split_line[i])
	{
		if ((ft_strcmp(split_line[i], "<") == 0 || ft_strcmp(split_line[i], "<<") == 0) &&
			(split_line[i + 1] == NULL || is_redirection(split_line[i + 1])))
		{
			printf("bash: syntax error near unexpected token `%s`\n", split_line[i + 1] ? split_line[i + 1] : "newline");
			return (EXIT_FAILURE);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}*/
