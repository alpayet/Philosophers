/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_philos.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 19:28:31 by alpayet           #+#    #+#             */
/*   Updated: 2025/08/08 13:48:28 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	init_philo_config(t_philo *philos, size_t index, t_data *data);
static bool	init_philo_semaphore(t_philo *philo);
static bool	creat_philo_process(t_philo *philos, size_t index);
void		end_the_simulation(size_t philo_nb, t_data *data);
void		trigger_the_end(t_data *data);
void		cleanup_data_semaphores(t_data *data);
void		cleanup_one_philo_semaphore(t_philo *philo);
void		cleanup_philos_semaphore(size_t philo_nb, t_philo *philos);
static void	cleanup_philos(size_t philo_initialize_nb,
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
		init_philo_config(philos, i, data);
		if (init_philo_semaphore(&(philos[i])) == false)
		{
			cleanup_philos(i, philos);
			return (NULL);
		}
		if (creat_philo_process(philos, i) == false)
		{
			cleanup_one_philo_semaphore(&(philos[i]));
			cleanup_philos(i, philos);
			return (NULL);
		}
		i++;
	}
	return (philos);
}

static void	init_philo_config(t_philo *philos, size_t index, t_data *data)
{
	philos[index].philo_id = index + 1;
	philos[index].data = data;
	philos[index].meals_count = 0;
	philos[index].should_exit = false;
}

static bool	init_philo_semaphore(t_philo *philo)
{
	t_error_type	error;

	philo->should_exit_mutex = open_new_sem_file(
			SEM_FILE_SHOULD_EXIT_MUTEX, 1);
	error = philo->should_exit_mutex.error;
	if (error != NO_ERROR)
	{
		if (error == SEM_OPEN)
			return (print_error(ERROR_SEM_OPEN));
		if (error == MALLOC)
			return (print_error(ERROR_MALLOC));
	}
	return (true);
}

static bool	creat_philo_process(t_philo *philos, size_t index)
{
	pthread_t	thread_id;

	philos[index].process_pid = fork();
	if (philos[index].process_pid == 0)
	{
		if (pthread_create(&thread_id, NULL,
				thread_routine, &(philos[index])) == 0)
		{
			process_routine(&(philos[index]));
			pthread_join(thread_id, NULL);
			cleanup_one_philo_semaphore(&(philos[index]));
			full_cleanup(index, NULL, philos);
			exit(EXIT_SUCCESS);
		}
		sem_wait(philos->data->print_access_mutex.ptr);
		trigger_the_end(philos->data);
		sem_post(philos->data->sem_threads_end_barrier.ptr);
		cleanup_one_philo_semaphore(&(philos[index]));
		full_cleanup(index, NULL, philos);
		exit(EXIT_FAILURE);
	}
	else if (philos[index].process_pid == -1)
		return (false);
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
