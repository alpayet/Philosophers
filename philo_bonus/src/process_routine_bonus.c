/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_routine_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 22:53:35 by alpayet           #+#    #+#             */
/*   Updated: 2025/07/28 00:34:59 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
void	*thread_routine(void *arg);

void	philo_log(t_philo *philo, char *str)
{
	milliseconds_t timestamp;

	if (str == NULL)
		return ;
	timestamp = get_current_time_in_ms() - philo->data->simulation_start_time;
	printf("%ld %zu %s", timestamp, philo->philo_id, str);
}

t_simulation_state	monitor_simulation(t_philo *philo, char *str)
{
	sem_wait(philo->data->mutex_print_access.ptr);
	sem_wait(philo->mutex_should_exit.ptr);
	if (philo->should_exit == true)
	{
		sem_post(philo->mutex_should_exit.ptr);
		sem_post(philo->data->mutex_print_access.ptr);
		return (END_OF_SIMULATION);
	}
	sem_post(philo->mutex_should_exit.ptr);
	if (get_current_time_in_ms() - philo->last_time_eat >= philo->data->time_to_die)
	{
		philo_log(philo, PHILO_DEAD_MSG);
		sem_post(philo->data->sem_end_barrier.ptr);
		return (END_OF_SIMULATION);
	}
	philo_log(philo, str);
	sem_post(philo->data->mutex_print_access.ptr);
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
		if (monitor_simulation(philo, NULL) == END_OF_SIMULATION)
			return (END_OF_SIMULATION);
		elapsed_time = get_current_time_in_ms() - begin_time;
		usleep(500);
	}
	return (SIMULATION_CONTINUES);
}

t_simulation_state	philo_takes_forks(t_philo *philo)
{
	sem_wait(philo->data->mutex_forks_acquisition.ptr);
	sem_wait(philo->data->sem_forks_nb.ptr);
	if (monitor_simulation(philo, PHILO_TAKEN_FORK_MSG) == END_OF_SIMULATION)
	{
		sem_post(philo->data->mutex_forks_acquisition.ptr);
		return (END_OF_SIMULATION);
	}
	sem_wait(philo->data->sem_forks_nb.ptr);
	if (monitor_simulation(philo, PHILO_TAKEN_FORK_MSG) == END_OF_SIMULATION)
	{
		sem_post(philo->data->mutex_forks_acquisition.ptr);
		return (END_OF_SIMULATION);
	}
	sem_post(philo->data->mutex_forks_acquisition.ptr);
	return (SIMULATION_CONTINUES);
}

t_simulation_state	philo_eating(t_philo *philo)
{
	philo->last_time_eat = get_current_time_in_ms();
	if (monitor_simulation(philo, PHILO_EATING_MSG) == END_OF_SIMULATION)
		return (END_OF_SIMULATION);
	if (usleep_check(philo, philo->data->time_to_eat) == END_OF_SIMULATION)
		return (END_OF_SIMULATION);
	sem_post(philo->data->sem_forks_nb.ptr);
	sem_post(philo->data->sem_forks_nb.ptr);
	return (SIMULATION_CONTINUES);
}
t_simulation_state	philo_sleeping(t_philo *philo)
{
	if (monitor_simulation(philo, PHILO_SLEEPING_MSG) == END_OF_SIMULATION)
		return (END_OF_SIMULATION);
	if (usleep_check(philo, philo->data->time_to_sleep) == END_OF_SIMULATION)
		return (END_OF_SIMULATION);
	return (SIMULATION_CONTINUES);
}

t_simulation_state	philo_thinking(t_philo *philo)
{
	if (monitor_simulation(philo, PHILO_THINKING_MSG) == END_OF_SIMULATION)
		return (END_OF_SIMULATION);
	return (SIMULATION_CONTINUES);
}

void	process_routine(t_philo *philo)
{
	sem_wait(philo->data->sem_start_barrier.ptr);
	philo->last_time_eat = get_current_time_in_ms();
	while (monitor_simulation(philo, NULL) == SIMULATION_CONTINUES)
	{
		if (philo_takes_forks(philo) == END_OF_SIMULATION)
			return ;
		if (philo_eating(philo) == END_OF_SIMULATION)
			return ;
		if (philo_sleeping(philo) == END_OF_SIMULATION)
			return ;
		if (philo_thinking(philo) == END_OF_SIMULATION)
			return ;
	}
}

void	*thread_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	sem_wait(philo->sem_thread_start_barrier.ptr);
	sem_wait(philo->mutex_should_exit.ptr);
	philo->should_exit = true;
	sem_post(philo->mutex_should_exit.ptr);
	sem_post(philo->data->sem_forks_nb.ptr);
	sem_post(philo->sem_thread_end_barrier.ptr);
	return (NULL);
}