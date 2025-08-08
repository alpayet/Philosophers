/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 19:33:30 by alpayet           #+#    #+#             */
/*   Updated: 2025/08/08 01:02:12 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static bool			init_data_semaphores(t_data *data, size_t fork_nb);
static t_error_type	open_access_semaphores(t_data *data, size_t fork_nb);
static t_error_type	open_barrier_semaphores(t_data *data);
void				cleanup_data_semaphores(t_data *data);

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
	t_error_type	error;

	error = open_access_semaphores(data, fork_nb);
	if (error != NO_ERROR)
	{
		cleanup_data_semaphores(data);
		if (error == SEM_OPEN)
			return (print_error(ERROR_SEM_OPEN));
		if (error == MALLOC)
			return (print_error(ERROR_MALLOC));
	}
	error = open_barrier_semaphores(data);
	if (error != NO_ERROR)
	{
		cleanup_data_semaphores(data);
		if (error == SEM_OPEN)
			return (print_error(ERROR_SEM_OPEN));
		if (error == MALLOC)
			return (print_error(ERROR_MALLOC));
	}
	return (true);
}

static t_error_type	open_access_semaphores(t_data *data, size_t fork_nb)
{
	data->forks_acquisition_mutex = open_new_sem_file(
			SEM_FILE_FORKS_AQUISITION_MUTEX, 1);
	if (data->forks_acquisition_mutex.error != NO_ERROR)
		return (data->forks_acquisition_mutex.error);
	data->sem_forks_nb = open_new_sem_file(
			SEM_FILE_FORKS_NB, fork_nb);
	if (data->sem_forks_nb.error != NO_ERROR)
		return (data->sem_forks_nb.error);
	data->print_access_mutex = open_new_sem_file(
			SEM_FILE_PRINT_ACCESS_MUTEX, 1);
	if (data->print_access_mutex.error != NO_ERROR)
		return (data->print_access_mutex.error);
	return (NO_ERROR);
}

static t_error_type	open_barrier_semaphores(t_data *data)
{
	data->min_meals_count_barrier = open_new_sem_file(
			SEM_FILE_MIN_MEALS_COUNT_BARRIER, 0);
	if (data->min_meals_count_barrier.error != NO_ERROR)
		return (data->min_meals_count_barrier.error);
	data->sem_start_barrier = open_new_sem_file(
			SEM_FILE_SIMU_START_BARRIER, 0);
	if (data->sem_start_barrier.error != NO_ERROR)
		return (data->sem_start_barrier.error);
	data->sem_end_barrier = open_new_sem_file(
			SEM_FILE_SIMU_END_BARRIER, 0);
	if (data->sem_end_barrier.error != NO_ERROR)
		return (data->sem_end_barrier.error);
	data->sem_threads_start_barrier = open_new_sem_file(
			SEM_FILE_THREAD_START_BARRIER, 0);
	if (data->sem_threads_start_barrier.error != NO_ERROR)
		return (data->sem_threads_start_barrier.error);
	data->sem_threads_end_barrier = open_new_sem_file(
			SEM_FILE_THREAD_END_BARRIER, 0);
	if (data->sem_threads_end_barrier.error != NO_ERROR)
		return (data->sem_threads_end_barrier.error);
	return (NO_ERROR);
}
