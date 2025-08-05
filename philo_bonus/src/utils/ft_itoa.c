/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 19:37:51 by alpayet           #+#    #+#             */
/*   Updated: 2025/07/31 16:43:05 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static int	ft_len_int(long n)
{
	int	count;

	count = 1;
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
