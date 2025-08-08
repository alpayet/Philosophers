/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 19:44:40 by alpayet           #+#    #+#             */
/*   Updated: 2025/08/08 00:00:48 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <sys/time.h>

bool	print_error(char *str)
{
	write(2, str, ft_strlen(str));
	return (false);
}

bool	check_malloc(void *ptr)
{
	if (ptr == NULL)
		return (print_error(ERROR_MALLOC));
	return (true);
}

t_milliseconds	get_current_time_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}
