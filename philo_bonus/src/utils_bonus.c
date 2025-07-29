/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 06:58:33 by alpayet           #+#    #+#             */
/*   Updated: 2025/07/28 09:35:34 by alpayet          ###   ########.fr       */
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

static int	ft_len_int(long n)
{
	int	count = 1;

	while (n >= 10)
	{
		n /= 10;
		count++;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	char	*n_str;
	long	n_long;
	int		len;
	int		is_negative;

	n_long = n;
	is_negative = 0;
	if (n_long < 0)
	{
		is_negative = 1;
		n_long = -n_long;
	}
	len = ft_len_int(n_long) + is_negative;
	n_str = malloc((len + 1) * sizeof(char));
	if (!n_str)
		return (NULL);
	n_str[len] = '\0';
	while (len-- != 0)
	{
		n_str[len] = (n_long % 10) + '0';
		n_long /= 10;
	}
	if (is_negative)
		n_str[0] = '-';
	return (n_str);
}
