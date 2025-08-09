/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_philos.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 19:28:31 by alpayet           #+#    #+#             */
/*   Updated: 2025/08/09 19:56:56 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static bool	init_philos(size_t philo_nb, t_philo *philos, t_data *data);
bool		creat_philos_process(size_t philo_nb, t_philo *philos);
static void	init_philo_config(t_philo *philos, size_t index, t_data *data);
static bool	init_philo_semaphore(t_philo *philo);

t_philo	*create_philos(size_t philo_nb, t_data *data)
{
	t_philo	*philos;

	philos = ft_calloc(philo_nb, sizeof(*philos));
	if (check_malloc(philos) == false)
		return (NULL);
	if (init_philos(philo_nb, philos, data) == false)
		return (NULL);
	data->simulation_start_time = get_current_time_in_ms();
	if (creat_philos_process(philo_nb, philos) == false)
		return (NULL);
	return (philos);
}

static bool	init_philos(size_t philo_nb, t_philo *philos, t_data *data)
{
	size_t	i;

	i = 0;
	while (i < philo_nb)
	{
		init_philo_config(philos, i, data);
		if (init_philo_semaphore(&(philos[i])) == false)
		{
			full_cleanup(i, NULL, philos);
			return (false);
		}
		i++;
	}
	return (true);
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
	philo->should_exit_mutex = open_new_sem_file(
			SEM_FILE_SHOULD_EXIT_MUTEX, 1);
	if (philo->should_exit_mutex.ptr == SEM_FAILED)
		return (false);
	philo->last_time_eat_mutex = open_new_sem_file(
			SEM_FILE_LAST_TIME_EAT_MUTEX, 1);
	if (philo->last_time_eat_mutex.ptr == SEM_FAILED)
		return (false);
	return (true);
}
