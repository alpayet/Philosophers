/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_end_simulation.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 20:03:25 by alpayet           #+#    #+#             */
/*   Updated: 2025/08/09 16:54:39 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	start_the_simulation(size_t philo_nb, sem_t *sem_start_barrier)
{
	size_t	i;

	i = 0;
	while (i < philo_nb)
	{
		sem_post(sem_start_barrier);
		i++;
	}
}

void	end_the_simulation(size_t philo_nb, t_data *data)
{
	size_t	i;

	i = 0;
	while (i < philo_nb)
	{
		sem_post(data->sem_threads_start_barrier.ptr);
		sem_wait(data->sem_threads_end_barrier.ptr);
		i++;
	}
	while (i < 2 * philo_nb)
	{
		sem_post(data->print_access_mutex.ptr);
		i++;
	}
}
