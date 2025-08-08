/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 19:16:25 by alpayet           #+#    #+#             */
/*   Updated: 2025/08/07 23:41:53 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void		cleanup_philos_semaphore(size_t philo_nb, t_philo *philos);
void		cleanup_one_philo_semaphore(t_philo *philo);
static void	secur_cleanup_semaphore(t_named_sem sem);
void		cleanup_data_semaphores(t_data *data);

void	full_cleanup(size_t philo_nb, t_data *data, t_philo *philos)
{
	if (philos != NULL)
		cleanup_philos_semaphore(philo_nb, philos);
	if (data != NULL)
		cleanup_data_semaphores(data);
	free(philos);
}

void	cleanup_philos_semaphore(size_t philo_nb, t_philo *philos)
{
	size_t	i;

	i = 0;
	while (i < philo_nb)
	{
		cleanup_one_philo_semaphore(&(philos[i]));
		i++;
	}
}

void	cleanup_one_philo_semaphore(t_philo *philo)
{
	secur_cleanup_semaphore(philo->should_exit_mutex);
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
	secur_cleanup_semaphore(data->forks_acquisition_mutex);
	secur_cleanup_semaphore(data->min_meals_count_barrier);
	secur_cleanup_semaphore(data->print_access_mutex);
	secur_cleanup_semaphore(data->sem_start_barrier);
	secur_cleanup_semaphore(data->sem_end_barrier);
}
