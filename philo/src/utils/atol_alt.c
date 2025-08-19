/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   atol_alt.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 19:45:32 by alpayet           #+#    #+#             */
/*   Updated: 2025/08/09 22:27:11 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <limits.h>

long	atol_alt(char *nptr)
{
	int		sign;
	size_t	n;

	if (*nptr == '\0')
		return (-1);
	sign = 1;
	if ((*nptr == '-' || *nptr == '+') && nptr[1] != '\0')
	{
		if (*nptr == '-')
			sign = -sign;
		nptr++;
	}
	n = 0;
	while (*nptr >= '0' && *nptr <= '9')
	{
		n = 10 * n + *nptr - '0';
		if ((sign == 1 && n > LONG_MAX)
			|| (sign == -1 && n > (size_t)LONG_MAX + 1))
			return (-1);
		nptr++;
	}
	if (*nptr != '\0')
		return (-1);
	return (sign * n);
}
