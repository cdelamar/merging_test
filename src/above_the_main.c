/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   above_the_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laubry <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 13:29:09 by laubry            #+#    #+#             */
/*   Updated: 2024/10/30 13:31:25 by laubry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token *copy_token_list(t_token *laubry_list)
{
    t_token *copy_head = NULL;
    t_token *copy_current = NULL;
    t_token *current = laubry_list;

    while (current != NULL)
    {
        t_token *new_node = malloc(sizeof(t_token));
        new_node->index = current->index;
        new_node->next = NULL;
        if (copy_head == NULL)
        {
            copy_head = new_node;
            copy_current = copy_head;
        }
        else
        {
            copy_current->next = new_node;
            copy_current = copy_current->next;
        }
        current = current->next;
    }
    return (copy_head);
}

char	*ft_realloc_string(char *str, int new_size)
{
	char	*res;
	int		old_len;

	if (!str)
		return (NULL);
	old_len = ft_strlen(str);
	res = malloc(old_len + new_size + 1);
	if (!res)
	{
		free(str);
		return (NULL);
	}
	ft_memcpy(res, str, old_len);
	res[old_len] = '\0';
	free(str);
	return (res);
}

char	*tab_to_str(char **tab)
{
	char	*str;
	int		i;

	str = malloc(1);
	i = -1;
	if (!str)
		return (NULL);
	str[0] = '\0';

	while (tab[++i])
	{
		str = ft_realloc_string(str, ft_strlen(tab[i]) + 2);
		if (!str)
			return (NULL);
		ft_strcat(str, tab[i]);
		if (tab[i + 1])
			ft_strcat(str, " ");
	}
	return (str);
}

int	add_node(t_token **token_list, char **strs, int i)
{
	t_token	*new_node;

	new_node = token_lstnew(strs[i]);
	if (new_node == NULL)
		return (check_error(ERROR_NODE));
	new_node->index = i;
	token_lstadd_back(token_list, new_node);
	return (1);
}

int	make_token(char **split_line, t_token **token_list)
{
	int	i;
	int	argc;

	argc = nbr_of_strs(split_line);
	if (argc == 0)
		return (0);
	i = 0;
	while (i < argc)
	{
		if (!add_node(token_list, split_line, i))
			return (0);
		i++;
	}
	lexer(*token_list);
	return (1);
}