/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 19:42:11 by alpayet           #+#    #+#             */
/*   Updated: 2025/07/31 16:44:00 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

char	*ft_strdup(char *s)
{
	char	*temp;
	size_t	index;
	size_t	size;

	size = ft_strlen(s);
	temp = (char *)malloc(sizeof(char) * (size + 1));
	if (!temp)
		return (NULL);
	index = 0;
	while (s[index])
	{
		temp[index] = s[index];
		index++;
	}
	temp[index] = '\0';
	return (temp);
}
