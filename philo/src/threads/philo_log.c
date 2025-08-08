/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_log.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 20:33:38 by alpayet           #+#    #+#             */
/*   Updated: 2025/08/07 23:45:16 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>

t_sim_state	philo_log(t_philo *philo, char *str)
{
	pthread_mutex_lock(&(philo->data->timestamp_mutex));
	if (is_simulation_ended(philo) == true)
	{
		pthread_mutex_unlock(&(philo->data->timestamp_mutex));
		return (END_OF_SIMULATION);
	}
	printf("%ld %zu %s", philo->data->timestamp, philo->philo_id, str);
	pthread_mutex_unlock(&(philo->data->timestamp_mutex));
	return (SIMULATION_CONTINUES);
}
