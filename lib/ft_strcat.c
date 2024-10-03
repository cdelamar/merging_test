/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdelamar <cdelamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 03:04:43 by cdelamar          #+#    #+#             */
/*   Updated: 2024/10/03 03:05:38 by cdelamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char    *ft_strcat(char *dest, char *src)
{
	int i = -1;
	int j = -1;

	while(dest[++i]) {}
	while(src[++j])
		dest[i + j] = src[j];
	dest[i + j] = 0;
	return dest;
}