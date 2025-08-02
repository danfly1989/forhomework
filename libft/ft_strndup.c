/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daflynn <daflynn@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 15:32:56 by daflynn           #+#    #+#             */
/*   Updated: 2025/08/02 17:02:28 by daflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char    *ft_strndup(const char *str, size_t n)
{
    if (!str || n == 0)
        return NULL;

    char    *ret = malloc(n + 1);
    size_t  i = 0;

    if (!ret)
        return NULL;

    while (i < n && str[i] != '\0')
    {
        ret[i] = str[i];
        i++;
    }
    ret[i] = '\0';
    return ret;
}

