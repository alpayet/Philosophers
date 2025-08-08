/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_check.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:40:32 by alpayet           #+#    #+#             */
/*   Updated: 2025/08/07 23:45:46 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	is_simulation_started(t_philo *philo);

t_sim_state	wait_all_philos(t_philo *philo)
{
	while (is_simulation_started(philo) == false)
	{
		if (is_simulation_ended(philo) == true)
			return (END_OF_SIMULATION);
		usleep(500);
	}
	return (SIMULATION_CONTINUES);
}

static bool	is_simulation_started(t_philo *philo)
{
	pthread_mutex_lock(&(philo->data->simulation_start_mutex));
	if (philo->data->simulation_start_time != -1)
	{
		pthread_mutex_unlock(&(philo->data->simulation_start_mutex));
		return (true);
	}
	pthread_mutex_unlock(&(philo->data->simulation_start_mutex));
	return (false);
}

bool	is_simulation_ended(t_philo *philo)
{
	pthread_mutex_lock(&(philo->data->simulation_end_mutex));
	if (philo->data->end_of_simulation == true)
	{
		pthread_mutex_unlock(&(philo->data->simulation_end_mutex));
		return (true);
	}
	pthread_mutex_unlock(&(philo->data->simulation_end_mutex));
	return (false);
}

t_sim_state	usleep_check(t_philo *philo, t_milliseconds sleep_time)
{
	t_milliseconds	begin_time;
	t_milliseconds	elapsed_time;

	begin_time = get_current_time_in_ms();
	elapsed_time = 0;
	while (elapsed_time < sleep_time)
	{
		if (is_simulation_ended(philo) == true)
			return (END_OF_SIMULATION);
		elapsed_time = get_current_time_in_ms() - begin_time;
		usleep(500);
	}
	return (SIMULATION_CONTINUES);
}
