/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_end_simulation.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 20:03:25 by alpayet           #+#    #+#             */
/*   Updated: 2025/07/31 01:22:16 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <sys/wait.h>

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

void	end_the_simulation(size_t philo_nb, t_philo *philos)
{
	size_t	i;

	i = 0;
	while (i < philo_nb)
	{
		sem_post(philos[i].sem_thread_start_barrier.ptr);
		sem_wait(philos[i].sem_thread_end_barrier.ptr);
		i++;
	}
	sem_post(philos->data->mutex_print_access.ptr);
}

void	wait_philos(size_t philo_nb, t_philo *philos)
{
	size_t	i;

	i = 0;
	while (i < philo_nb)
	{
		waitpid(philos[i].process_pid, NULL, 0);
		i++;
	}
}
