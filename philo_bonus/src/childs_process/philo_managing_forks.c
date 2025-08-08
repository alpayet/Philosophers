/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_managing_forks.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 20:37:09 by alpayet           #+#    #+#             */
/*   Updated: 2025/08/07 23:39:42 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

t_sim_state	philo_takes_forks(t_philo *philo)
{
	sem_wait(philo->data->forks_acquisition_mutex.ptr);
	sem_wait(philo->data->sem_forks_nb.ptr);
	if (monitor_simulation(philo, PHILO_TAKEN_FORK_MSG) == END_OF_SIMULATION)
	{
		sem_post(philo->data->forks_acquisition_mutex.ptr);
		return (END_OF_SIMULATION);
	}
	sem_wait(philo->data->sem_forks_nb.ptr);
	if (monitor_simulation(philo, PHILO_TAKEN_FORK_MSG) == END_OF_SIMULATION)
	{
		sem_post(philo->data->forks_acquisition_mutex.ptr);
		return (END_OF_SIMULATION);
	}
	sem_post(philo->data->forks_acquisition_mutex.ptr);
	return (SIMULATION_CONTINUES);
}

void	philo_drops_forks(t_philo *philo)
{
	sem_post(philo->data->sem_forks_nb.ptr);
	sem_post(philo->data->sem_forks_nb.ptr);
}
