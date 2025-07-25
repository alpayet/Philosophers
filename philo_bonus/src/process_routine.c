/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_routine.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 22:53:35 by alpayet           #+#    #+#             */
/*   Updated: 2025/07/25 08:53:09 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

bool	is_simulation_ended(t_philo *philo)
{
	sem_wait(philo->data->mutex_simulation_end);
	if (philo->data->end_of_simulation == true)
	{
		sem_post(philo->data->mutex_simulation_end);
		return (true);
	}
	sem_post(philo->data->mutex_simulation_end);
	return (false);
}

t_return	philo_log(t_philo *philo, char *str)
{
	milliseconds_t timestamp;

	timestamp = get_current_time_in_ms() - philo->data->simulation_start_time;
	if (is_simulation_ended(philo) == true)
		return (END_OF_SIMULATION);
	printf("%ld %zu %s", timestamp, philo->philo_id, str);
	return (RETURN_SUCCESS);
}

t_return	philo_takes_forks(t_philo *philo)
{
	sem_wait(philo->data->mutex_forks_acquisition);
	sem_wait(philo->data->sem_forks_nb);
	sem_post(philo->data->sem_forks_nb);
	if (philo_log(philo, PHILO_TAKEN_FORK_MSG) == END_OF_SIMULATION)
		return (END_OF_SIMULATION);
	sem_wait(philo->data->sem_forks_nb);
	sem_post(philo->data->sem_forks_nb);
	if (philo_log(philo, PHILO_TAKEN_FORK_MSG) == END_OF_SIMULATION)
		return (END_OF_SIMULATION);
	sem_post(philo->data->mutex_forks_acquisition);
	return (RETURN_SUCCESS);
}

t_return	philo_eating(t_philo *philo)
{
	philo->last_time_eat = get_current_time_in_ms();
	if (philo_log(philo, PHILO_EATING_MSG) == END_OF_SIMULATION)
		return (END_OF_SIMULATION);
	if (usleep_check(philo, philo->data->time_to_eat) == END_OF_SIMULATION)
		return (END_OF_SIMULATION);
	pthread_mutex_lock(&(philo->fork_left->mutex));
	philo->fork_left->state = AVAILABLE;
	pthread_mutex_unlock(&(philo->fork_left->mutex));
	pthread_mutex_lock(&(philo->fork_right->mutex));
	philo->fork_right->state = AVAILABLE;
	pthread_mutex_unlock(&(philo->fork_right->mutex));
	return (RETURN_SUCCESS);
}

void	process_routine(t_philo *philo)
{
	pthread_t thread_id;

	pthread_create(&thread_id, NULL, thread_routine, philo->data);
	pthread_detach(thread_id);
	sem_wait(philo->data->sem_blocking);
	while (is_simulation_ended(philo) == false)
	{
		if (philo_takes_forks(philo) == END_OF_SIMULATION)
			return (NULL);
		if (philo_eating(philo) == END_OF_SIMULATION)
			return (NULL);
		if (philo_sleeping(philo) == END_OF_SIMULATION)
			return (NULL);
		if (philo_thinking(philo) == END_OF_SIMULATION)
			return (NULL);
	}
	return (NULL);

	sem_wait(sem_mutex);
	sem_wait(sem_forks_nb);

	sem_post(sem_forks_nb);
	sem_wait(sem_forks_nb);
	sem_post(sem_forks_nb);
	sem_post(sem_mutex);
}

void	*thread_routine(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	sem_wait(data->sem_blocking);
	sem_wait(data->mutex_simulation_end);
	data->end_of_simulation = true;
	sem_post(data->mutex_simulation_end);
	return (NULL);
}