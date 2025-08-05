/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_philos.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 19:28:31 by alpayet           #+#    #+#             */
/*   Updated: 2025/08/05 18:06:42 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static bool			init_philo_semaphores(t_philo *philo);
static t_error_type	open_philo_semaphores(t_philo *philo);
static bool			creat_philo_process(t_philo *philos, size_t index);
void				*thread_routine(void *arg);
void				cleanup_one_philo_semaphores(t_philo *philo);
static t_philo		*cleanup_philos_return_null(size_t philo_initialize_nb,
						t_philo *philos);

t_philo	*create_philos(size_t philo_nb, t_data *data)
{
	size_t	i;
	t_philo	*philos;

	philos = ft_calloc(philo_nb, sizeof(*philos));
	if (check_malloc(philos) == false)
		return (NULL);
	data->simulation_start_time = get_current_time_in_ms();
	i = 0;
	while (i < philo_nb)
	{
		philos[i].philo_id = i + 1;
		philos[i].data = data;
		philos[i].meals_count = 0;
		philos[i].should_exit = false;
		if (init_philo_semaphores(&(philos[i])) == false)
			return (cleanup_philos_return_null(i, philos));
		if (creat_philo_process(philos, i) == false)
		{
			cleanup_one_philo_semaphores(&(philos[i]));
			return (cleanup_philos_return_null(i, philos));
		}
		i++;
	}
	return (philos);
}

static bool	init_philo_semaphores(t_philo *philo)
{
	t_error_type	error;

	error = open_philo_semaphores(philo);
	if (error != NO_ERROR)
	{
		cleanup_one_philo_semaphores(philo);
		if (error == SEM_OPEN)
			print_error(ERROR_SEM_OPEN);
		if (error == MALLOC)
			print_error(ERROR_MALLOC);
		return (false);
	}
	return (true);
}

static t_error_type	open_philo_semaphores(t_philo *philo)
{
	philo->mutex_should_exit = open_new_sem_file(
			SEM_FILE_SHOULD_EXIT_MUTEX, 1);
	if (philo->mutex_should_exit.error != NO_ERROR)
		return (philo->mutex_should_exit.error);
	philo->sem_thread_start_barrier = open_new_sem_file(
			SEM_FILE_THREAD_START_BARRIER, 0);
	if (philo->sem_thread_start_barrier.error != NO_ERROR)
		return (philo->sem_thread_start_barrier.error);
	philo->sem_thread_end_barrier = open_new_sem_file(
			SEM_FILE_THREAD_END_BARRIER, 0);
	if (philo->sem_thread_end_barrier.error != NO_ERROR)
		return (philo->sem_thread_end_barrier.error);
	return (NO_ERROR);
}

static bool	creat_philo_process(t_philo *philos, size_t index)
{
	pthread_t	thread_id;

	philos[index].process_pid = fork();
	if (philos[index].process_pid == 0)
	{
		if (pthread_create(&thread_id, NULL,
			thread_routine,&(philos[index])) != 0)
		{
			cleanup_one_philo_semaphores(&(philos[index]));
			full_cleanup(index, philos->data, philos);
			exit(EXIT_FAILURE);
		}
		process_routine(&(philos[index]));
		pthread_join(thread_id, NULL);
		cleanup_one_philo_semaphores(&(philos[index]));
		full_cleanup(index, philos->data, philos);
		exit(EXIT_SUCCESS);
	}
	else if (philos[index].process_pid == -1)
		return (false);
	return (true);
}

static t_philo	*cleanup_philos_return_null(size_t philo_initialized_nb,
					t_philo *philos)
{
	start_the_simulation(philo_initialized_nb,
		philos->data->sem_start_barrier.ptr);
	end_the_simulation(philo_initialized_nb, philos);
	full_cleanup(philo_initialized_nb, NULL, philos);
	return (NULL);
}
