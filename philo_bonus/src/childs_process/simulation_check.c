/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_check.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 20:36:22 by alpayet           #+#    #+#             */
/*   Updated: 2025/08/09 17:32:43 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

t_sim_state	is_simulation_ended(t_philo *philo)
{
	sem_wait(philo->should_exit_mutex.ptr);
	if (philo->should_exit == true)
	{
		sem_post(philo->should_exit_mutex.ptr);
		return (END_OF_SIMULATION);
	}
	sem_post(philo->should_exit_mutex.ptr);
	return (SIMULATION_CONTINUES);
}

t_sim_state	usleep_check(t_philo *philo, t_milliseconds sleep_time)
{
	t_milliseconds	begin_time;
	t_milliseconds	elapsed_time;

	begin_time = get_current_time_in_ms();
	elapsed_time = 0;
	while (elapsed_time < sleep_time)
	{
		if (is_simulation_ended(philo) == END_OF_SIMULATION)
			return (END_OF_SIMULATION);
		elapsed_time = get_current_time_in_ms() - begin_time;
		usleep(500);
	}
	return (SIMULATION_CONTINUES);
}
