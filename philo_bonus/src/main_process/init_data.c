/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 19:33:30 by alpayet           #+#    #+#             */
/*   Updated: 2025/08/08 23:13:43 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static bool	init_data_semaphores(t_data *data, size_t fork_nb);
static bool	open_access_semaphores(t_data *data, size_t fork_nb);
static bool	open_barrier_semaphores(t_data *data);

bool	init_data(t_data *data, long argv_long[5])
{
	data->philo_nb = argv_long[0];
	data->time_to_die = argv_long[1];
	data->time_to_eat = argv_long[2];
	data->time_to_sleep = argv_long[3];
	data->min_meals_count = argv_long[4];
	if (init_data_semaphores(data, argv_long[0]) == false)
		return (false);
	return (true);
}

static bool	init_data_semaphores(t_data *data, size_t fork_nb)
{
	if (open_access_semaphores(data, fork_nb) == false)
	{
		cleanup_data_semaphores(data);
		return (false);
	}
	if (open_barrier_semaphores(data) == false)
	{
		cleanup_data_semaphores(data);
		return (false);
	}
	return (true);
}

static bool	open_access_semaphores(t_data *data, size_t fork_nb)
{
	data->forks_acquisition_mutex = open_new_sem_file(
			SEM_FILE_FORKS_AQUISITION_MUTEX, 1);
	if (data->forks_acquisition_mutex.ptr == SEM_FAILED)
		return (false);
	data->sem_forks_nb = open_new_sem_file(
			SEM_FILE_FORKS_NB, fork_nb);
	if (data->sem_forks_nb.ptr == SEM_FAILED)
		return (false);
	data->print_access_mutex = open_new_sem_file(
			SEM_FILE_PRINT_ACCESS_MUTEX, 1);
	if (data->print_access_mutex.ptr == SEM_FAILED)
		return (false);
	return (true);
}

static bool	open_barrier_semaphores(t_data *data)
{
	data->min_meals_count_barrier = open_new_sem_file(
			SEM_FILE_MIN_MEALS_COUNT_BARRIER, 0);
	if (data->min_meals_count_barrier.ptr == SEM_FAILED)
		return (false);
	data->sem_start_barrier = open_new_sem_file(
			SEM_FILE_SIMU_START_BARRIER, 0);
	if (data->sem_start_barrier.ptr == SEM_FAILED)
		return (false);
	data->sem_end_barrier = open_new_sem_file(
			SEM_FILE_SIMU_END_BARRIER, 0);
	if (data->sem_end_barrier.ptr == SEM_FAILED)
		return (false);
	data->sem_threads_start_barrier = open_new_sem_file(
			SEM_FILE_THREAD_START_BARRIER, 0);
	if (data->sem_threads_start_barrier.ptr == SEM_FAILED)
		return (false);
	data->sem_threads_end_barrier = open_new_sem_file(
			SEM_FILE_THREAD_END_BARRIER, 0);
	if (data->sem_threads_end_barrier.ptr == SEM_FAILED)
		return (false);
	return (true);
}
