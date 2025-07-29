/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_routine.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 03:21:35 by alpayet           #+#    #+#             */
/*   Updated: 2025/07/27 23:17:19 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	is_simulation_started(t_philo *philo)
{
	pthread_mutex_lock(&(philo->data->mutex_simulation_start));
	if (philo->data->simulation_start_time != -1)
	{
		pthread_mutex_unlock(&(philo->data->mutex_simulation_start));
		return (true);
	}
	pthread_mutex_unlock(&(philo->data->mutex_simulation_start));
	return (false);
}

bool	is_simulation_ended(t_philo *philo)
{
	pthread_mutex_lock(&(philo->data->mutex_simulation_end));
	if (philo->data->end_of_simulation == true)
	{
		pthread_mutex_unlock(&(philo->data->mutex_simulation_end));
		return (true);
	}
	pthread_mutex_unlock(&(philo->data->mutex_simulation_end));
	return (false);
}

t_simulation_state	wait_all_philos(t_philo *philo)
{
	while (is_simulation_started(philo) == false)
	{
		if (is_simulation_ended(philo) == true)
			return (END_OF_SIMULATION);
		usleep(500);
	}
	return (SIMULATION_CONTINUES);
}

t_simulation_state	usleep_check(t_philo *philo, milliseconds_t sleep_time)
{
	milliseconds_t begin_time;
	milliseconds_t elapsed_time;

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

t_simulation_state	philo_log(t_philo *philo, char *str)
{
	pthread_mutex_lock(&(philo->data->mutex_timestamp));
	if (is_simulation_ended(philo) == true)
	{
		pthread_mutex_unlock(&(philo->data->mutex_timestamp));
		return (END_OF_SIMULATION);
	}
	printf("%ld %zu %s", philo->data->timestamp, philo->philo_id, str);
	pthread_mutex_unlock(&(philo->data->mutex_timestamp));
	return (SIMULATION_CONTINUES);
}

bool	fork_available(t_fork *fork)
{
	pthread_mutex_lock(&(fork->mutex));
	if (fork->state == AVAILABLE)
	{
		fork->state = NOT_AVAILABLE;
		pthread_mutex_unlock(&(fork->mutex));
		return (true);
	}
	pthread_mutex_unlock(&(fork->mutex));
	return (false);
}

t_simulation_state	philo_thinking(t_philo *philo)
{
	if (philo_log(philo, PHILO_THINKING_MSG) == END_OF_SIMULATION)
		return (END_OF_SIMULATION);
	if (philo->data->philo_nb % 2 != 0)
		usleep_check(philo, philo->data->time_to_eat);
	return (SIMULATION_CONTINUES);
}

t_simulation_state	wait_for_fork(t_philo *philo, t_fork *fork)
{
	while (fork_available(fork) == false)
	{
		if (is_simulation_ended(philo) == true)
			return (END_OF_SIMULATION);
		usleep(500);
	}
	return (SIMULATION_CONTINUES);
}

t_simulation_state	philo_odd_takes_forks(t_philo *philo)
{
	if (wait_for_fork(philo, philo->fork_right) == END_OF_SIMULATION)
		return (END_OF_SIMULATION);
	if (philo_log(philo, PHILO_TAKEN_FORK_MSG) == END_OF_SIMULATION)
		return (END_OF_SIMULATION);
	if (wait_for_fork(philo, philo->fork_left) == END_OF_SIMULATION)
		return (END_OF_SIMULATION);
	if (philo_log(philo, PHILO_TAKEN_FORK_MSG) == END_OF_SIMULATION)
		return (END_OF_SIMULATION);
	return (SIMULATION_CONTINUES);
}

t_simulation_state	philo_even_takes_forks(t_philo *philo)
{
	if (wait_for_fork(philo, philo->fork_left) == END_OF_SIMULATION)
		return (END_OF_SIMULATION);
	if (philo_log(philo, PHILO_TAKEN_FORK_MSG) == END_OF_SIMULATION)
		return (END_OF_SIMULATION);
	if (wait_for_fork(philo, philo->fork_right) == END_OF_SIMULATION)
		return (END_OF_SIMULATION);
	if (philo_log(philo, PHILO_TAKEN_FORK_MSG) == END_OF_SIMULATION)
		return (END_OF_SIMULATION);
	return (SIMULATION_CONTINUES);
}

t_simulation_state	philo_sleeping(t_philo *philo)
{
	if (philo_log(philo, PHILO_SLEEPING_MSG) == END_OF_SIMULATION)
		return (END_OF_SIMULATION);
	if (usleep_check(philo, philo->data->time_to_sleep) == END_OF_SIMULATION)
		return (END_OF_SIMULATION);
	return (SIMULATION_CONTINUES);
}

t_simulation_state	philo_eating(t_philo *philo)
{
	pthread_mutex_lock(&(philo->mutex_last_time_eat));
	philo->last_time_eat = get_current_time_in_ms();
	pthread_mutex_unlock(&(philo->mutex_last_time_eat));
	if (philo_log(philo, PHILO_EATING_MSG) == END_OF_SIMULATION)
		return (END_OF_SIMULATION);
	if (philo->data->min_meals_count != -1)
	{
		pthread_mutex_lock(&(philo->mutex_meals_count));
		(philo->meals_count)++;
		pthread_mutex_unlock(&(philo->mutex_meals_count));
	}
	if (usleep_check(philo, philo->data->time_to_eat) == END_OF_SIMULATION)
		return (END_OF_SIMULATION);
	pthread_mutex_lock(&(philo->fork_left->mutex));
	philo->fork_left->state = AVAILABLE;
	pthread_mutex_unlock(&(philo->fork_left->mutex));
	pthread_mutex_lock(&(philo->fork_right->mutex));
	philo->fork_right->state = AVAILABLE;
	pthread_mutex_unlock(&(philo->fork_right->mutex));
	return (SIMULATION_CONTINUES);
}

void	*thread_odd_routine(void *arg)
{
	t_philo *philo;

	philo = (t_philo *)arg;
	if (wait_all_philos(philo) == END_OF_SIMULATION)
		return(NULL);
	while (is_simulation_ended(philo) == false)
	{
		if (philo_odd_takes_forks(philo) == END_OF_SIMULATION)
			return (NULL);
		if (philo_eating(philo) == END_OF_SIMULATION)
			return (NULL);
		if (philo_sleeping(philo) == END_OF_SIMULATION)
			return (NULL);
		if (philo_thinking(philo) == END_OF_SIMULATION)
			return (NULL);
	}
	return (NULL);
}

t_simulation_state	create_offset(t_philo *philo)
{
	if (philo_log(philo, PHILO_THINKING_MSG) == END_OF_SIMULATION)
		return (END_OF_SIMULATION);
	usleep_check(philo, philo->data->time_to_eat);
	return (SIMULATION_CONTINUES);
}

void	*thread_even_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (wait_all_philos(philo) == END_OF_SIMULATION)
		return(NULL);
	create_offset(philo);
	while (is_simulation_ended(philo) == false)
	{
		if (philo_even_takes_forks(philo) == END_OF_SIMULATION)
			return (NULL);
		if (philo_eating(philo) == END_OF_SIMULATION)
			return (NULL);
		if (philo_sleeping(philo) == END_OF_SIMULATION)
			return (NULL);
		if (philo_thinking(philo) == END_OF_SIMULATION)
			return (NULL);
	}
	return (NULL);
}
