/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 19:42:46 by alpayet           #+#    #+#             */
/*   Updated: 2025/07/31 16:44:05 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

char	*ft_strjoin(char *s1, char *s2)
{
	char	*temp;
	size_t	index;
	size_t	pre_size;

	if (!s1 || !s2)
		return (NULL);
	pre_size = ft_strlen(s1);
	temp = (char *)malloc(sizeof(char) * (pre_size + ft_strlen(s2) + 1));
	if (!temp)
		return (NULL);
	index = 0;
	while (s1[index])
	{
		temp[index] = s1[index];
		index++;
	}
	index = 0;
	while (s2[index])
	{
		temp[pre_size + index] = s2[index];
		index++;
	}
	temp[pre_size + index] = '\0';
	return (temp);
}
