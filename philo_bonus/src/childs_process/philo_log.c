/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_log.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 17:32:16 by alpayet           #+#    #+#             */
/*   Updated: 2025/08/09 19:26:08 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <stdio.h>

t_sim_state	philo_log(t_philo *philo, char *str, t_philo_state philo_state)
{
	t_milliseconds	timestamp;

	sem_wait(philo->data->print_access_mutex.ptr);
	if (is_simulation_ended(philo) == END_OF_SIMULATION)
	{
		sem_post(philo->data->print_access_mutex.ptr);
		return (END_OF_SIMULATION);
	}
	timestamp = get_current_time_in_ms() - philo->data->simulation_start_time;
	printf("%ld %zu %s", timestamp, philo->philo_id, str);
	if (philo_state == ALIVE)
		sem_post(philo->data->print_access_mutex.ptr);
	return (SIMULATION_CONTINUES);
}
