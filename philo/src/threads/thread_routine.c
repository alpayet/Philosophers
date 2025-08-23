/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_routine.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 03:21:35 by alpayet           #+#    #+#             */
/*   Updated: 2025/08/23 22:37:53 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_sim_state			wait_all_philos(t_philo *philo);
static t_sim_state	create_offset(t_philo *philo);
t_sim_state			philo_thinking(t_philo *philo);
t_sim_state			philo_odd_takes_forks(t_philo *philo);
t_sim_state			philo_even_takes_forks(t_philo *philo);
t_sim_state			philo_eating(t_philo *philo,
						t_sim_state (*philo_takes_forks)(t_philo *philo));
t_sim_state			philo_sleeping(t_philo *philo);
static void			control_scheduler(t_philo *philo);

void	*thread_odd_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (wait_all_philos(philo) == END_OF_SIMULATION)
		return (NULL);
	if (philo_thinking(philo) == END_OF_SIMULATION)
			return (NULL);
	while (is_simulation_ended(philo) == false)
	{
		if (philo_eating(philo, philo_odd_takes_forks) == END_OF_SIMULATION)
			return (NULL);
		if (philo_sleeping(philo) == END_OF_SIMULATION)
			return (NULL);
		if (philo_thinking(philo) == END_OF_SIMULATION)
			return (NULL);
		control_scheduler(philo);
	}
	return (NULL);
}

void	*thread_even_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (wait_all_philos(philo) == END_OF_SIMULATION)
		return (NULL);
	if (philo_thinking(philo) == END_OF_SIMULATION)
		return (NULL);
	create_offset(philo);
	while (is_simulation_ended(philo) == false)
	{
		if (philo_eating(philo, philo_even_takes_forks) == END_OF_SIMULATION)
			return (NULL);
		if (philo_sleeping(philo) == END_OF_SIMULATION)
			return (NULL);
		if (philo_thinking(philo) == END_OF_SIMULATION)
			return (NULL);
		control_scheduler(philo);
	}
	return (NULL);
}

t_sim_state	create_offset(t_philo *philo)
{
	usleep_check(philo, philo->data->time_to_eat);
	return (SIMULATION_CONTINUES);
}

void	control_scheduler(t_philo *philo)
{
	if (philo->data->philo_nb % 2 != 0)
		usleep_check(philo, philo->data->time_to_eat);
}
