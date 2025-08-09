/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 11:55:48 by alpayet           #+#    #+#             */
/*   Updated: 2025/08/09 16:43:43 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	cleanup_threads(size_t thread_nb, t_philo *philos);
void		destroy_philos_mutexes(size_t philo_nb, t_philo *philos);
void		destroy_forks_mutex(size_t fork_nb, t_fork *forks);
static void	destroy_data_mutexes(t_data *data);

void	full_cleanup(size_t	thread_nb, t_fork *forks,
	t_data *data, t_philo *philos)
{
	if (philos != NULL)
	{
		cleanup_threads(thread_nb, philos);
		destroy_philos_mutexes(thread_nb, philos);
	}
	if (forks != NULL)
		destroy_forks_mutex(thread_nb, forks);
	if (data != NULL)
		destroy_data_mutexes(data);
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

void	destroy_philos_mutexes(size_t philo_nb, t_philo *philos)
{
	size_t	i;

	i = 0;
	while (i < philo_nb)
	{
		pthread_mutex_destroy(&(philos[i].mutex_last_time_eat));
		pthread_mutex_destroy(&(philos[i].mutex_meals_count));
		i++;
	}
}

void	destroy_data_mutexes(t_data *data)
{
	pthread_mutex_destroy(&(data->timestamp_mutex));
	pthread_mutex_destroy(&(data->simulation_start_mutex));
	pthread_mutex_destroy(&(data->simulation_end_mutex));
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
