/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_routine.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 20:30:35 by alpayet           #+#    #+#             */
/*   Updated: 2025/08/05 16:05:25 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

t_sim_state	philo_takes_forks(t_philo *philo);
t_sim_state	philo_eating(t_philo *philo);
t_sim_state	philo_sleeping(t_philo *philo);
t_sim_state	philo_thinking(t_philo *philo);

void	process_routine(t_philo *philo)
{
	sem_wait(philo->data->sem_start_barrier.ptr);
	philo->last_time_eat = get_current_time_in_ms();
	while (monitor_simulation(philo, NULL) == SIMULATION_CONTINUES)
	{
		if (philo_eating(philo) == END_OF_SIMULATION)
			return ;
		if (philo_sleeping(philo) == END_OF_SIMULATION)
			return ;
		if (philo_thinking(philo) == END_OF_SIMULATION)
			return ;
	}
}
