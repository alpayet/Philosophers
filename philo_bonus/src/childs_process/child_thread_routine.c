/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_thread_routine.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 20:41:13 by alpayet           #+#    #+#             */
/*   Updated: 2025/08/05 15:56:04 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*thread_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	sem_wait(philo->sem_thread_start_barrier.ptr);
	sem_wait(philo->mutex_should_exit.ptr);
	philo->should_exit = true;
	sem_post(philo->mutex_should_exit.ptr);
	sem_post(philo->data->sem_forks_nb.ptr);
	sem_post(philo->sem_thread_end_barrier.ptr);
	return (NULL);
}
