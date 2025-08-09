/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_thread_routines.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 20:41:13 by alpayet           #+#    #+#             */
/*   Updated: 2025/08/09 18:30:05 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	trigger_the_end(t_data *data);

void	*wait_for_ending(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	sem_wait(philo->data->sem_threads_start_barrier.ptr);
	sem_wait(philo->should_exit_mutex.ptr);
	philo->should_exit = true;
	sem_post(philo->should_exit_mutex.ptr);
	sem_post(philo->data->sem_forks_nb.ptr);
	sem_post(philo->data->sem_threads_end_barrier.ptr);
	return (NULL);
}

void	*update_philo_last_time_eat(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (is_simulation_ended(philo) == SIMULATION_CONTINUES)
	{
		sem_wait(philo->last_time_eat_mutex.ptr);
		if (get_current_time_in_ms() - philo->last_time_eat
			>= philo->data->time_to_die)
		{
			sem_post(philo->last_time_eat_mutex.ptr);
			philo_log(philo, PHILO_DEAD_MSG, DEAD);
			trigger_the_end(philo->data);
			return (NULL);
		}
		sem_post(philo->last_time_eat_mutex.ptr);
		usleep(1500);
	}
	return (NULL);
}
