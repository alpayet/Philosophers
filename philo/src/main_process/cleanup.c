/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 11:55:48 by alpayet           #+#    #+#             */
/*   Updated: 2025/07/31 01:36:34 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	cleanup_threads(size_t thread_nb, t_philo *philos);
static void	destroy_philos_mutexes(size_t philo_nb, t_philo *philos);
void		destroy_one_philo_mutexes(t_philo *philo);
void		destroy_forks_mutex(size_t fork_nb, t_fork *forks);

void	full_cleanup(size_t	thread_nb, t_fork *forks,
	t_data *data, t_philo *philos)
{
	if (philos != NULL)
	{
		cleanup_threads(thread_nb, philos);
		destroy_philos_mutexes(thread_nb, philos);
	}
	if (data != NULL)
	{
		pthread_mutex_destroy(&(data->mutex_timestamp));
		pthread_mutex_destroy(&(data->mutex_simulation_start));
		pthread_mutex_destroy(&(data->mutex_simulation_end));
	}
	if (forks != NULL)
		destroy_forks_mutex(thread_nb, forks);
	free(forks);
	free(philos);
}

static void	cleanup_threads(size_t thread_nb, t_philo *philos)
{
	size_t	i;

	i = 0;
	while (i < thread_nb)
	{
		pthread_join(philos[i].thread_id, NULL);
		i++;
	}
}

static void	destroy_philos_mutexes(size_t philo_nb, t_philo *philos)
{
	size_t	i;

	i = 0;
	while (i < philo_nb)
	{
		destroy_one_philo_mutexes(&(philos[i]));
		i++;
	}
}

void	destroy_one_philo_mutexes(t_philo *philo)
{
	pthread_mutex_destroy(&(philo->mutex_last_time_eat));
	pthread_mutex_destroy(&(philo->mutex_meals_count));
}

void	destroy_forks_mutex(size_t fork_nb, t_fork *forks)
{
	size_t	i;

	i = 0;
	while (i < fork_nb)
	{
		pthread_mutex_destroy(&(forks[i].mutex));
		i++;
	}
}
