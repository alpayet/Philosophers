/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_philos.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:08:33 by alpayet           #+#    #+#             */
/*   Updated: 2025/08/09 17:16:41 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	init_philos(size_t philo_nb, t_fork *forks,
				t_philo *philos, t_data *data);
t_philo		*creat_philos_thread(size_t philo_nb, t_philo *philos);
static void	init_philo_forks(t_philo *philos, size_t index,
				t_fork *forks, size_t philo_nb);
static void	init_philo_config(t_philo *philos, size_t index, t_data *data);
static bool	init_philo_mutexes(t_philo *philo);

t_philo	*create_philos(size_t philo_nb, t_fork *forks, t_data *data)
{
	t_philo	*philos;

	philos = ft_calloc(philo_nb, sizeof(*philos));
	if (check_malloc(philos) == false)
		return (NULL);
	if (init_philos(philo_nb, forks, philos, data) == false)
		return (NULL);
	if (creat_philos_thread(philo_nb, philos) == false)
		return (NULL);
	return (philos);
}

static bool	init_philos(size_t philo_nb, t_fork *forks,
	t_philo *philos, t_data *data)
{
	size_t	i;

	i = 0;
	while (i < philo_nb)
	{
		init_philo_forks(philos, i, forks, philo_nb);
		init_philo_config(philos, i, data);
		if (init_philo_mutexes(&(philos[i])) == false)
		{
			destroy_philos_mutexes(i, philos);
			return (false);
		}
		i++;
	}
	return (true);
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
