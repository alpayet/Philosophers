/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_routine.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 03:21:35 by alpayet           #+#    #+#             */
/*   Updated: 2025/07/18 09:59:19 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	is_simulation_started(t_philo *philo)
{
	pthread_mutex_lock(&(philo->data->mutex_simulation_start));
	if (philo->data->start_of_simulation == true)
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

t_return	wait_all_philos(t_philo *philo)
{
	while (is_simulation_started(philo) == false)
	{
		if (is_simulation_ended(philo) == true)
			return (END_OF_SIMULATION);
		usleep(500);
	}
	return (RETURN_SUCESS);
}

t_return	usleep_check(t_philo *philo, suseconds_t sleep_time)
{
	struct timeval tv;
	suseconds_t begin_time;
	suseconds_t current_time;
	suseconds_t elapsed_time;

	gettimeofday(&tv, NULL);
	begin_time = tv.tv_sec * 1000000 + tv.tv_usec;
	elapsed_time = 0;
	while (elapsed_time < sleep_time)
	{
		if (is_simulation_ended(philo) == true)
			return (END_OF_SIMULATION);
		gettimeofday(&tv, NULL);
		current_time = tv.tv_sec * 1000000 + tv.tv_usec;
		elapsed_time = current_time - begin_time;
		usleep(500);
	}
	return (RETURN_SUCESS);
}

t_return	philo_log(t_philo *philo, char *str)
{
	if (is_simulation_ended(philo) == true)
		return (END_OF_SIMULATION);
	pthread_mutex_lock(&(philo->data->mutex_timestamp));
	printf("%ld %zu %s", philo->data->timestamp / 1000, philo->philo_id, str);
	pthread_mutex_unlock(&(philo->data->mutex_timestamp));
	return (RETURN_SUCESS);
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

t_return	philo_thinking(t_philo *philo)
{
	if (philo_log(philo, PHILO_THINKING_MSG) == END_OF_SIMULATION)
		return (END_OF_SIMULATION);
	usleep_check(philo, philo->data->time_to_eat);
	return (RETURN_SUCESS);
}

t_return	wait_for_fork(t_philo *philo, t_fork *fork)
{
	while (1)
	{
		if (is_simulation_ended(philo) == true)
			return (END_OF_SIMULATION);
		if (fork_available(fork) == true)
		{
			if (philo_log(philo, PHILO_TAKEN_FORK_MSG) == END_OF_SIMULATION)
				return (END_OF_SIMULATION);
			break;
		}
		usleep(500);
	}
	return (RETURN_SUCESS);
}

t_return	philo_odd_takes_forks(t_philo *philo)
{
	if (fork_available(philo->fork_right) == false)
		return (RETURN_FAILURE);
	if (philo_log(philo, PHILO_TAKEN_FORK_MSG) == END_OF_SIMULATION)
		return (END_OF_SIMULATION);
	if (wait_for_fork(philo, philo->fork_left) == END_OF_SIMULATION)
		return (END_OF_SIMULATION);
	return (RETURN_SUCESS);
}

t_return	philo_even_takes_forks(t_philo *philo)
{
	if (fork_available(philo->fork_left) == false)
		return (RETURN_FAILURE);
	if (philo_log(philo, PHILO_TAKEN_FORK_MSG) == END_OF_SIMULATION)
		return (END_OF_SIMULATION);
	if (wait_for_fork(philo, philo->fork_right) == END_OF_SIMULATION)
		return (END_OF_SIMULATION);
	return (RETURN_SUCESS);
}

t_return	philo_sleeping(t_philo *philo)
{
	if (philo_log(philo, PHILO_SLEEPING_MSG) == END_OF_SIMULATION)
		return (END_OF_SIMULATION);
	if (usleep_check(philo, philo->data->time_to_sleep) == END_OF_SIMULATION)
		return (END_OF_SIMULATION);
	return (RETURN_SUCESS);
}

t_return	philo_eating(t_philo *philo)
{
	struct timeval tv;

	pthread_mutex_lock(&(philo->mutex_last_time_eat));
	gettimeofday(&tv, NULL);
	philo->last_time_eat = tv.tv_sec * 1000000 + tv.tv_usec;
	pthread_mutex_unlock(&(philo->mutex_last_time_eat));
	if (philo_log(philo, PHILO_EATING_MSG) == END_OF_SIMULATION)
		return (END_OF_SIMULATION);
	pthread_mutex_lock(&(philo->mutex_meals_eaten_nb));
	(philo->meals_eaten_nb)++;
	pthread_mutex_unlock(&(philo->mutex_meals_eaten_nb));
	if (usleep_check(philo, philo->data->time_to_eat) == END_OF_SIMULATION)
		return (END_OF_SIMULATION);
	pthread_mutex_lock(&(philo->fork_left->mutex));
	philo->fork_left->state = AVAILABLE;
	pthread_mutex_unlock(&(philo->fork_left->mutex));
	pthread_mutex_lock(&(philo->fork_right->mutex));
	philo->fork_right->state = AVAILABLE;
	pthread_mutex_unlock(&(philo->fork_right->mutex));
	return (RETURN_SUCESS);
}

void	*thread_odd_routine(void *arg)
{
	t_philo *philo;
	t_return return_status;

	philo = (t_philo *)arg;
	if (wait_all_philos(philo) == END_OF_SIMULATION)
		return(NULL);
	while (1)
	{
		if (is_simulation_ended(philo) == true)
			return (NULL);
		return_status = philo_odd_takes_forks(philo);
		if (return_status == END_OF_SIMULATION)
			return (NULL);
		if (return_status == RETURN_FAILURE)
		{
			usleep(500);
			continue;
		}
		if (philo_eating(philo) == END_OF_SIMULATION)
			return (NULL);
		if (philo_sleeping(philo) == END_OF_SIMULATION)
			return (NULL);
		if (philo_thinking(philo) == END_OF_SIMULATION)
			return (NULL);
	}
}

void	*thread_even_routine(void *arg)
{
	t_philo *philo;
	t_return return_status;

	philo = (t_philo *)arg;
	if (wait_all_philos(philo) == END_OF_SIMULATION)
		return(NULL);
	usleep_check(philo, philo->data->time_to_eat);
	while (1)
	{
		if (is_simulation_ended(philo) == true)
			return (NULL);
		return_status = philo_even_takes_forks(philo);
		if (return_status == END_OF_SIMULATION)
			return (NULL);
		if (return_status == RETURN_FAILURE)
		{
			usleep(500);
			continue;
		}
		if (philo_eating(philo) == END_OF_SIMULATION)
			return (NULL);
		if (philo_sleeping(philo) == END_OF_SIMULATION)
			return (NULL);
		if (philo_thinking(philo) == END_OF_SIMULATION)
			return (NULL);
	}
}
