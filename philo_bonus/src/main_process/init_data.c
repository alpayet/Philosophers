/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 19:33:30 by alpayet           #+#    #+#             */
/*   Updated: 2025/08/05 16:03:16 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static bool			init_data_semaphores(t_data *data, size_t fork_nb);
void				cleanup_data_semaphores(t_data *data);
static t_error_type	open_data_semaphores(t_data *data, size_t fork_nb);

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

	error = open_data_semaphores(data, fork_nb);
	if (error != NO_ERROR)
	{
		cleanup_data_semaphores(data);
		if (error == SEM_OPEN)
			print_error(ERROR_SEM_OPEN);
		if (error == MALLOC)
			print_error(ERROR_MALLOC);
		return (false);
	}
	return (true);
}

static t_error_type	open_data_semaphores(t_data *data, size_t fork_nb)
{
	data->mutex_forks_acquisition = open_new_sem_file(
			SEM_FILE_FORKS_AQUISITION_MUTEX, 1);
	if (data->mutex_forks_acquisition.error != NO_ERROR)
		return (data->mutex_forks_acquisition.error);
	data->sem_forks_nb = open_new_sem_file(SEM_FILE_FORKS_NB, fork_nb);
	if (data->sem_forks_nb.error != NO_ERROR)
		return (data->sem_forks_nb.error);
	data->min_meals_count_barrier = open_new_sem_file(
			SEM_FILE_MIN_MEALS_COUNT_BARRIER, 0);
	if (data->min_meals_count_barrier.error != NO_ERROR)
		return (data->min_meals_count_barrier.error);
	data->mutex_print_access = open_new_sem_file(
			SEM_FILE_PRINT_ACCESS_MUTEX, 1);
	if (data->mutex_print_access.error != NO_ERROR)
		return (data->mutex_print_access.error);
	data->sem_start_barrier = open_new_sem_file(
			SEM_FILE_SIMU_START_BARRIER, 0);
	if (data->sem_start_barrier.error != NO_ERROR)
		return (data->sem_start_barrier.error);
	data->sem_end_barrier = open_new_sem_file(
			SEM_FILE_SIMU_END_BARRIER, 0);
	if (data->sem_end_barrier.error != NO_ERROR)
		return (data->sem_end_barrier.error);
	return (NO_ERROR);
}
