/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_check.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 20:36:22 by alpayet           #+#    #+#             */
/*   Updated: 2025/08/07 23:41:53 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	trigger_the_end(t_data *data);

t_sim_state	monitor_simulation(t_philo *philo, char *str)
{
	sem_wait(philo->data->print_access_mutex.ptr);
	sem_wait(philo->should_exit_mutex.ptr);
	if (philo->should_exit == true)
	{
		sem_post(philo->should_exit_mutex.ptr);
		sem_post(philo->data->print_access_mutex.ptr);
		return (END_OF_SIMULATION);
	}
	sem_post(philo->should_exit_mutex.ptr);
	if (get_current_time_in_ms() - philo->last_time_eat
		>= philo->data->time_to_die)
	{
		philo_log(philo, PHILO_DEAD_MSG);
		trigger_the_end(philo->data);
		return (END_OF_SIMULATION);
	}
	philo_log(philo, str);
	sem_post(philo->data->print_access_mutex.ptr);
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
		if (monitor_simulation(philo, NULL) == END_OF_SIMULATION)
			return (END_OF_SIMULATION);
		elapsed_time = get_current_time_in_ms() - begin_time;
		usleep(500);
	}
	return (SIMULATION_CONTINUES);
}
