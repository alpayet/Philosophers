/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 19:16:25 by alpayet           #+#    #+#             */
/*   Updated: 2025/07/31 16:13:18 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	cleanup_philos_semaphores(size_t philo_nb, t_philo *philos);
void		cleanup_one_philo_semaphores(t_philo *philo);
static void	secur_cleanup_semaphore(t_named_sem sem);
void		cleanup_data_semaphores(t_data *data);
void		wait_philos(size_t philo_nb, t_philo *philos);

void	full_cleanup(size_t philo_nb, t_data *data, t_philo *philos)
{
	if (philos != NULL)
	{
		wait_philos(philo_nb, philos);
		cleanup_philos_semaphores(philo_nb, philos);
	}
	if (data != NULL)
		cleanup_data_semaphores(data);
	free(philos);
}

static void	cleanup_philos_semaphores(size_t philo_nb, t_philo *philos)
{
	size_t	i;

	i = 0;
	while (i < philo_nb)
	{
		cleanup_one_philo_semaphores(&(philos[i]));
		i++;
	}
}

void	cleanup_one_philo_semaphores(t_philo *philo)
{
	secur_cleanup_semaphore(philo->mutex_should_exit);
	secur_cleanup_semaphore(philo->sem_thread_start_barrier);
	secur_cleanup_semaphore(philo->sem_thread_end_barrier);
}

static void	secur_cleanup_semaphore(t_named_sem sem)
{
	if (sem.ptr != NULL && sem.ptr != SEM_FAILED)
	{
		sem_close(sem.ptr);
		sem_unlink(sem.name);
		free(sem.name);
	}
}

void	cleanup_data_semaphores(t_data *data)
{
	secur_cleanup_semaphore(data->sem_forks_nb);
	secur_cleanup_semaphore(data->mutex_forks_acquisition);
	secur_cleanup_semaphore(data->min_meals_count_barrier);
	secur_cleanup_semaphore(data->mutex_print_access);
	secur_cleanup_semaphore(data->sem_start_barrier);
	secur_cleanup_semaphore(data->sem_end_barrier);
}
