/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safety.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laubry <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 16:48:35 by laubry            #+#    #+#             */
/*   Updated: 2024/10/30 20:10:18 by laubry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool	space_only(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] != ' ' && line[i] != '\t')
			return (false);
		i++;
	}
	return (true);
}

void	print_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		printf("array %d : %s\n", i, tab[i]);
		i++;
	}
}

bool	freeable_tab(char **tab)
{
	size_t	i;

	i = 0;
	while (tab[i])
		i++;
	if (i <= 1)
		return (false);
	return (true);
}

void	free_cmd(t_cmd *cmd)
{
    if (cmd)
    {
        if (cmd->path_command)
        {
            ft_freetab(cmd->path_command);
            cmd->path_command = NULL;  // Réinitialise après libération
        }
        if (cmd->path_split)
        {
            ft_freetab(cmd->path_split);
            cmd->path_split = NULL;  // Réinitialise après libération
        }
        if (cmd->final_tab)
        {
            ft_freetab(cmd->final_tab);
            cmd->final_tab = NULL;  // Réinitialise après libération
        }
        free(cmd);
        cmd = NULL;  // Réinitialise après libération
    }
}

char *get_token_type(t_token_enum type)
{
	if (type == WORD)
		return "WORD (Command)";
	else if (type == SPACES)
		return "SPACES";
	else if (type == PIPE)
		return "PIPE (|)";
	else if (type == REDIRECTION)
		return "REDIRECTION";
	else if (type == VAR)
		return "VAR ($)";
	else if (type == DOUBLE_QUOTE)
		return "DOUBLE_QUOTE";
	else if (type == SIMPLE_QUOTE)
		return "SIMPLE_QUOTE";
	else if (type == LAST_VERIF)
		return "LAST_VERIF";
	else if (type == GLOBAL)
		return "GLOBAL ($?)";
	else if (type == ERROR)
		return "ERROR";
	else if (type == END)
		return "END (\\0)";
	else
		return "UNKNOWN";
}

void print_token_list(t_token *head)
{
	t_token *current = head;

	while (current != NULL)
	{
		printf("Index: %d\n", current->index);
		printf("Type: %s\n", get_token_type(current->type));
		printf("Content: %s\n", current->content);
		printf("\n");

		current = current->next; // Move to the next token
	}
}
