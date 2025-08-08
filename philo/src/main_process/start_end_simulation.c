/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_end_simulation.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 20:02:22 by alpayet           #+#    #+#             */
/*   Updated: 2025/08/07 23:45:46 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	start_the_simulation(size_t philo_nb, t_philo *philos)
{
	t_milliseconds	simulation_start_time;
	size_t			i;

	simulation_start_time = get_current_time_in_ms();
	i = 0;
	while (i < philo_nb)
	{
		philos[i].last_time_eat = simulation_start_time;
		i++;
	}
	pthread_mutex_lock(&(philos->data->simulation_start_mutex));
	philos->data->simulation_start_time = simulation_start_time;
	pthread_mutex_unlock(&(philos->data->simulation_start_mutex));
}

void	end_the_simulation(t_data *data)
{
	pthread_mutex_lock(&(data->simulation_end_mutex));
	data->end_of_simulation = true;
	pthread_mutex_unlock(&(data->simulation_end_mutex));
}
