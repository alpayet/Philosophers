/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_thread_routines.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 20:41:13 by alpayet           #+#    #+#             */
/*   Updated: 2025/08/08 13:48:13 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*thread_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	sem_wait(philo->data->sem_threads_start_barrier.ptr);
	sem_wait(philo->should_exit_mutex.ptr);
	philo->should_exit = true;
	sem_post(philo->should_exit_mutex.ptr);
	sem_post(philo->data->sem_forks_nb.ptr);
	sem_post(philo->data->sem_threads_end_barrier.ptr);
	return (NULL);
}
