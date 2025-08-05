/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 20:28:34 by alpayet           #+#    #+#             */
/*   Updated: 2025/08/05 16:07:22 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

t_sim_state	philo_takes_forks(t_philo *philo);
void		philo_drops_forks(t_philo *philo);

t_sim_state	philo_eating(t_philo *philo)
{
	if (philo_takes_forks(philo) == END_OF_SIMULATION)
		return (END_OF_SIMULATION);
	philo->last_time_eat = get_current_time_in_ms();
	if (monitor_simulation(philo, PHILO_EATING_MSG) == END_OF_SIMULATION)
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
	if (monitor_simulation(philo, PHILO_SLEEPING_MSG) == END_OF_SIMULATION)
		return (END_OF_SIMULATION);
	if (usleep_check(philo, philo->data->time_to_sleep) == END_OF_SIMULATION)
		return (END_OF_SIMULATION);
	return (SIMULATION_CONTINUES);
}

t_sim_state	philo_thinking(t_philo *philo)
{
	if (monitor_simulation(philo, PHILO_THINKING_MSG) == END_OF_SIMULATION)
		return (END_OF_SIMULATION);
	return (SIMULATION_CONTINUES);
}
