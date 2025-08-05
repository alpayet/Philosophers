/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:33:33 by alpayet           #+#    #+#             */
/*   Updated: 2025/08/05 16:10:54 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_drops_forks(t_philo *philo);

t_sim_state	philo_thinking(t_philo *philo)
{
	if (philo_log(philo, PHILO_THINKING_MSG) == END_OF_SIMULATION)
		return (END_OF_SIMULATION);
	return (SIMULATION_CONTINUES);
}

t_sim_state	philo_sleeping(t_philo *philo)
{
	if (philo_log(philo, PHILO_SLEEPING_MSG) == END_OF_SIMULATION)
		return (END_OF_SIMULATION);
	if (usleep_check(philo, philo->data->time_to_sleep) == END_OF_SIMULATION)
		return (END_OF_SIMULATION);
	return (SIMULATION_CONTINUES);
}

t_sim_state	philo_eating(t_philo *philo,
	t_sim_state (*philo_takes_forks)(t_philo *philo))
{
	if (philo_takes_forks(philo) == END_OF_SIMULATION)
		return (END_OF_SIMULATION);
	pthread_mutex_lock(&(philo->mutex_last_time_eat));
	philo->last_time_eat = get_current_time_in_ms();
	pthread_mutex_unlock(&(philo->mutex_last_time_eat));
	if (philo_log(philo, PHILO_EATING_MSG) == END_OF_SIMULATION)
		return (END_OF_SIMULATION);
	if (philo->data->min_meals_count != -1)
	{
		pthread_mutex_lock(&(philo->mutex_meals_count));
		(philo->meals_count)++;
		pthread_mutex_unlock(&(philo->mutex_meals_count));
	}
	if (usleep_check(philo, philo->data->time_to_eat) == END_OF_SIMULATION)
		return (END_OF_SIMULATION);
	philo_drops_forks(philo);
	return (SIMULATION_CONTINUES);
}
