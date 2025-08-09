/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 20:28:34 by alpayet           #+#    #+#             */
/*   Updated: 2025/08/09 18:29:12 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

t_sim_state	philo_takes_forks(t_philo *philo);
void		philo_drops_forks(t_philo *philo);

t_sim_state	philo_eating(t_philo *philo)
{
	if (philo_takes_forks(philo) == END_OF_SIMULATION)
		return (END_OF_SIMULATION);
	sem_wait(philo->last_time_eat_mutex.ptr);
	philo->last_time_eat = get_current_time_in_ms();
	sem_post(philo->last_time_eat_mutex.ptr);
	if (philo_log(philo, PHILO_EATING_MSG, ALIVE) == END_OF_SIMULATION)
		return (END_OF_SIMULATION);
	if (philo->data->min_meals_count != -1)
	{
		(philo->meals_count)++;
		if (philo->meals_count == philo->data->min_meals_count)
			sem_post(philo->data->min_meals_count_barrier.ptr);
	}
	if (usleep_check(philo, philo->data->time_to_eat) == END_OF_SIMULATION)
		return (END_OF_SIMULATION);
	philo_drops_forks(philo);
	return (SIMULATION_CONTINUES);
}

t_sim_state	philo_sleeping(t_philo *philo)
{
	if (philo_log(philo, PHILO_SLEEPING_MSG, ALIVE) == END_OF_SIMULATION)
		return (END_OF_SIMULATION);
	if (usleep_check(philo, philo->data->time_to_sleep) == END_OF_SIMULATION)
		return (END_OF_SIMULATION);
	return (SIMULATION_CONTINUES);
}

t_sim_state	philo_thinking(t_philo *philo)
{
	if (philo_log(philo, PHILO_THINKING_MSG, ALIVE) == END_OF_SIMULATION)
		return (END_OF_SIMULATION);
	return (SIMULATION_CONTINUES);
}
