/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_managing_forks.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:29:06 by alpayet           #+#    #+#             */
/*   Updated: 2025/08/05 16:06:56 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static t_sim_state	wait_for_fork(t_philo *philo, t_fork *fork);
static bool			fork_available(t_fork *fork);

t_sim_state	philo_odd_takes_forks(t_philo *philo)
{
	if (wait_for_fork(philo, philo->fork_right) == END_OF_SIMULATION)
		return (END_OF_SIMULATION);
	if (philo_log(philo, PHILO_TAKEN_FORK_MSG) == END_OF_SIMULATION)
		return (END_OF_SIMULATION);
	if (wait_for_fork(philo, philo->fork_left) == END_OF_SIMULATION)
		return (END_OF_SIMULATION);
	if (philo_log(philo, PHILO_TAKEN_FORK_MSG) == END_OF_SIMULATION)
		return (END_OF_SIMULATION);
	return (SIMULATION_CONTINUES);
}

t_sim_state	philo_even_takes_forks(t_philo *philo)
{
	if (wait_for_fork(philo, philo->fork_left) == END_OF_SIMULATION)
		return (END_OF_SIMULATION);
	if (philo_log(philo, PHILO_TAKEN_FORK_MSG) == END_OF_SIMULATION)
		return (END_OF_SIMULATION);
	if (wait_for_fork(philo, philo->fork_right) == END_OF_SIMULATION)
		return (END_OF_SIMULATION);
	if (philo_log(philo, PHILO_TAKEN_FORK_MSG) == END_OF_SIMULATION)
		return (END_OF_SIMULATION);
	return (SIMULATION_CONTINUES);
}

void	philo_drops_forks(t_philo *philo)
{
	pthread_mutex_lock(&(philo->fork_left->mutex));
	philo->fork_left->state = AVAILABLE;
	pthread_mutex_unlock(&(philo->fork_left->mutex));
	pthread_mutex_lock(&(philo->fork_right->mutex));
	philo->fork_right->state = AVAILABLE;
	pthread_mutex_unlock(&(philo->fork_right->mutex));
}

static t_sim_state	wait_for_fork(t_philo *philo, t_fork *fork)
{
	while (fork_available(fork) == false)
	{
		if (is_simulation_ended(philo) == true)
			return (END_OF_SIMULATION);
		usleep(500);
	}
	return (SIMULATION_CONTINUES);
}

static bool	fork_available(t_fork *fork)
{
	pthread_mutex_lock(&(fork->mutex));
	if (fork->state == AVAILABLE)
	{
		fork->state = NOT_AVAILABLE;
		pthread_mutex_unlock(&(fork->mutex));
		return (true);
	}
	pthread_mutex_unlock(&(fork->mutex));
	return (false);
}
