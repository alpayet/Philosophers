/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_childs_process.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 18:26:20 by alpayet           #+#    #+#             */
/*   Updated: 2025/08/19 23:05:37 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static bool	creat_one_philo_process(t_philo *philos, size_t index);
static bool	secur_create_philo_threads(t_philo *philos, size_t index,
				pthread_t *thread_wait_id, pthread_t *thread_update_id);
static bool	create_philo_threads(t_philo *philo,
				pthread_t *thread_wait_id, pthread_t *thread_update_id);
void		trigger_the_end(t_data *data);
static void	cleanup_philos(size_t philo_initialized_nb, t_philo *philos);

bool	creat_philos_process(size_t philo_nb, t_philo *philos)
{
	size_t	i;

	i = 0;
	while (i < philo_nb)
	{
		if (creat_one_philo_process(philos, i) == false)
		{
			cleanup_philos(philo_nb, philos);
			return (false);
		}
		i++;
	}
	return (true);
}

static bool	creat_one_philo_process(t_philo *philos, size_t index)
{
	pthread_t	thread_wait_id;
	pthread_t	thread_update_id;

	philos[index].process_pid = fork();
	if (philos[index].process_pid == 0)
	{
		sem_wait(philos->data->sem_start_barrier.ptr);
		philos[index].last_time_eat = get_current_time_in_ms();
		if (secur_create_philo_threads(philos, index,
				&thread_wait_id, &thread_update_id) == false)
		{
			print_error(ERROR_THREAD_CREATE);
			exit(EXIT_FAILURE);
		}
		process_routine(&(philos[index]));
		pthread_join(thread_wait_id, NULL);
		pthread_join(thread_update_id, NULL);
		full_cleanup(philos->data->philo_nb, philos->data, philos);
		exit(EXIT_SUCCESS);
	}
	else if (philos[index].process_pid == -1)
		return (false);
	return (true);
}

static bool	secur_create_philo_threads(t_philo *philos, size_t index,
	pthread_t *thread_wait_id, pthread_t *thread_update_id)
{
	if (create_philo_threads(&(philos[index]),
			thread_wait_id, thread_update_id) == false)
	{
		sem_wait(philos->data->print_access_mutex.ptr);
		trigger_the_end(philos->data);
		sem_post(philos->data->sem_threads_end_barrier.ptr);
		full_cleanup(philos->data->philo_nb, philos->data, philos);
		return (false);
	}
	return (true);
}

static bool	create_philo_threads(t_philo *philo,
	pthread_t *thread_wait_id, pthread_t *thread_update_id)
{
	if (pthread_create(thread_wait_id, NULL,
			wait_for_ending, philo) != 0)
		return (false);
	if (pthread_create(thread_update_id, NULL,
			update_philo_last_time_eat, philo) != 0)
	{
		sem_post(philo->data->sem_threads_start_barrier.ptr);
		pthread_join(*thread_wait_id, NULL);
		return (false);
	}
	return (true);
}

static void	cleanup_philos(size_t philo_initialized_nb, t_philo *philos)
{
	start_the_simulation(philo_initialized_nb,
		philos->data->sem_start_barrier.ptr);
	end_the_simulation(philo_initialized_nb, philos->data);
	wait_philos(philo_initialized_nb, philos);
	full_cleanup(philo_initialized_nb, NULL, philos);
}
