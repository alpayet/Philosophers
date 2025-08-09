/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_routine.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 20:30:35 by alpayet           #+#    #+#             */
/*   Updated: 2025/08/08 21:58:20 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

t_sim_state	philo_takes_forks(t_philo *philo);
t_sim_state	philo_eating(t_philo *philo);
t_sim_state	philo_sleeping(t_philo *philo);
t_sim_state	philo_thinking(t_philo *philo);

void	process_routine(t_philo *philo)
{
	while (is_simulation_ended(philo) == SIMULATION_CONTINUES)
	{
		if (philo_eating(philo) == END_OF_SIMULATION)
			return ;
		if (philo_sleeping(philo) == END_OF_SIMULATION)
			return ;
		if (philo_thinking(philo) == END_OF_SIMULATION)
			return ;
	}
}
