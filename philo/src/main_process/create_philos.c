/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_philos.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:08:33 by alpayet           #+#    #+#             */
/*   Updated: 2025/08/07 23:55:56 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	init_philo_forks(t_philo *philos, size_t index,
				t_fork *forks, size_t philo_nb);
static void	init_philo_config(t_philo *philos, size_t index, t_data *data);
static bool	init_philo_mutexes(t_philo *philo);
static bool	creat_philo_thread(t_philo *philo);
void		destroy_one_philo_mutexes(t_philo *philo);

t_philo	*create_philos(size_t philo_nb, t_fork *forks, t_data *data)
{
	t_philo	*philos;
	size_t	i;

	philos = malloc(philo_nb * sizeof(*philos));
	if (check_malloc(philos) == false)
		return (NULL);
	i = 0;
	while (i < philo_nb)
	{
		init_philo_forks(philos, i, forks, philo_nb);
		init_philo_config(philos, i, data);
		if (init_philo_mutexes(&(philos[i])) == false)
		{
			full_cleanup(philo_nb, NULL, NULL, philos);
			return (NULL);
		}
		if (creat_philo_thread(&(philos[i])) == false)
		{
			destroy_one_philo_mutexes(&(philos[i]));
			full_cleanup(philo_nb, NULL, NULL, philos);
			return (NULL);
		}
		i++;
	}
	return (philos);
}

static void	init_philo_forks(t_philo *philos, size_t index,
	t_fork *forks, size_t philo_nb)
{
	philos[index].fork_left = forks + index;
	philos[index].fork_right = forks + (index + 1) % philo_nb;
}

static void	init_philo_config(t_philo *philos, size_t index, t_data *data)
{
	philos[index].philo_id = index + 1;
	philos[index].data = data;
	philos[index].meals_count = 0;
}

static bool	init_philo_mutexes(t_philo *philo)
{
	if (pthread_mutex_init(&(philo->mutex_last_time_eat), NULL) != 0)
		return (print_error(ERROR_MUTEX_INIT));
	if (pthread_mutex_init(&(philo->mutex_meals_count), NULL) != 0)
	{
		pthread_mutex_destroy(&(philo->mutex_last_time_eat));
		return (print_error(ERROR_MUTEX_INIT));
	}
	return (true);
}

static bool	creat_philo_thread(t_philo *philo)
{
	void	*(*routine)(void *);

	if (philo->philo_id % 2 == 0)
		routine = thread_even_routine;
	else
		routine = thread_odd_routine;
	if (pthread_create(&(philo->thread_id), NULL, routine, philo) != 0)
	{
		end_the_simulation(philo->data);
		return (print_error(ERROR_THREAD_CREATE));
	}
	return (true);
}
