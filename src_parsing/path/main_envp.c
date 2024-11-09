/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_envp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laubry <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 18:51:59 by laubry            #+#    #+#             */
/*   Updated: 2024/11/09 14:02:53 by laubry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	getenv_in_list(char **envp, t_token **token)
{
	char		**env;
	size_t		len;
	char		*start;
	int			old_enum;
	t_token		*next;

	start = (*token)->content + 1;
	env = envp;
	len = ft_strlen((*token)->content) - 1;
	while (*env != NULL)
	{
		if (ft_strncmp(*env, start, len) == 0 && (*env)[len] == '=')
		{
			start = *env + len + 1;
			old_enum = (*token)->type;
			free((*token)->content);
			(*token)->content = ft_strdup(start);
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
	int		prefix;
	char	*signal_str;

	head = token_list;
	while ((*head)->index < place_of_dollar)
		head = &(*head)->next;
	if ((*head)->type == SIMPLE_QUOTE)
		return ;
	if (!ft_strcmp((*head)->content, "$?"))// || !ft_strcmp((*head)->content, "$"))
	{
		free((*head)->content);
		signal_str = malloc(12);
		signal_str = ft_itoa(g_signal);
		(*head)->content = signal_str;
		return ;
	}
	prefix = skip_prefix((*head)->content);
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
