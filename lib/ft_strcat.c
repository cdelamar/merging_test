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

char *ft_strcat(char *dest, char *src)
{
    int i = 0;
    int j = 0;

    // Find the end of dest string
    while (dest[i])
        i++;

    // Append src to dest
    while (src[j])
    {
        dest[i + j] = src[j];
        j++;
    }

    dest[i + j] = '\0'; // Null-terminate the concatenated string
    return dest;
}