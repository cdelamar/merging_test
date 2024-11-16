/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_envp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laubry <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 18:51:59 by laubry            #+#    #+#             */
/*   Updated: 2024/11/16 00:20:20 by laubry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*reste(char *start)
{
	int		i;
	int		j;
	char	*str;

	i = 0;
	j = 0;
	if (!start[i])
		return (start);
	str = ft_calloc(ft_strlen(start) + 1, sizeof(char));
	while (start[i])
	{
		if (!ft_isalnum(start[i]))
		{
			while (start[i])
			{
				str[j] = start[i];
				j++;
				i++;
			}
			str[j] = '\0';
			return (str);
		}
		i++;
	}
	return (str);
}

char	*before(char *start)
{
	int		i;
	char	*str;

	str = ft_calloc(ft_strlen(start) + 1, sizeof(char));
	i = 0;
	while (start[i])
	{
		if (!ft_isalnum(start[i]))
		{
			str[i] = '\0';
			return (str);
		}
		str[i] = start[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

void	getenv_in_list(char **envp, t_token **token)
{
	char		**env;
	size_t		len;
	char		*start_rest[2];
	int			old_enum;
	t_token		*next;

	start_rest[0] = before((*token)->content + 1);
	start_rest[1] = reste((*token)->content + 1);
	env = envp;
	len = ft_strlen(start_rest[0]);
	while (*env != NULL)
	{
		if (ft_strncmp(*env, start_rest[0], len) == 0 && (*env)[len] == '=')
		{
			free(start_rest[0]);
			start_rest[0] = *env + len + 1;
			old_enum = (*token)->type;
			free((*token)->content);
			if (start_rest[1] != NULL)
			{
				(*token)->content = ft_strjoin(start_rest[0], start_rest[1]);
				free(start_rest[1]);
				start_rest[1] = NULL;
			}
			else
				(*token)->content = ft_strdup(start_rest[0]);
			(*token)->type = old_enum;
			return ;
		}
		env++;
	}
	next = (*token)->next;
	free((*token)->content);
	free(*token);
	*token = next;
}

void	path_other(char **envp, t_token **token_list, int place_of_dollar)
{
	t_token	**head;
	char	*signal_str;

	head = token_list;
	while ((*head)->index < place_of_dollar)
		head = &(*head)->next;
	if ((*head)->type == SIMPLE_QUOTE)
		return ;
	if (!ft_strcmp((*head)->content, "$?"))
	{
		free((*head)->content);
		signal_str = malloc(12);
		signal_str = ft_itoa(g_signal);
		(*head)->content = signal_str;
		return ;
	}
	getenv_in_list(envp, head);
}

void	path_main(t_token **token_list, char **envp)
{
	t_token	**head;
	int		place_of_dollar;

	head = token_list;
	while (*head)
	{
		place_of_dollar = find_the_dollar(*head);
		if (place_of_dollar != -1)
			path_other(envp, token_list, place_of_dollar);
		if (*head)
			head = &(*head)->next;
	}
}
