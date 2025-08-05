/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_log.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 20:34:39 by alpayet           #+#    #+#             */
/*   Updated: 2025/07/31 02:16:24 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <stdio.h>

void	philo_log(t_philo *philo, char *str)
{
	t_milliseconds	timestamp;

	if (str == NULL)
		return ;
	timestamp = get_current_time_in_ms() - philo->data->simulation_start_time;
	printf("%ld %zu %s", timestamp, philo->philo_id, str);
}
