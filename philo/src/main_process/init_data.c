/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:10:15 by alpayet           #+#    #+#             */
/*   Updated: 2025/08/07 23:56:58 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	init_data_mutexes(t_data *data);

bool	init_data(t_data *data, long argv_long[5])
{
	data->philo_nb = argv_long[0];
	data->time_to_die = argv_long[1];
	data->time_to_eat = argv_long[2];
	data->time_to_sleep = argv_long[3];
	data->min_meals_count = argv_long[4];
	data->timestamp = 0;
	data->simulation_start_time = -1;
	data->end_of_simulation = false;
	if (init_data_mutexes(data) == false)
		return (false);
	return (true);
}

static bool	init_data_mutexes(t_data *data)
{
	if (pthread_mutex_init(&(data->timestamp_mutex), NULL) != 0)
		return (print_error(ERROR_MUTEX_INIT));
	if (pthread_mutex_init(&(data->simulation_start_mutex), NULL) != 0)
	{
		pthread_mutex_destroy(&(data->timestamp_mutex));
		return (print_error(ERROR_MUTEX_INIT));
	}
	if (pthread_mutex_init(&(data->simulation_end_mutex), NULL) != 0)
	{
		pthread_mutex_destroy(&(data->timestamp_mutex));
		pthread_mutex_destroy(&(data->simulation_start_mutex));
		return (print_error(ERROR_MUTEX_INIT));
	}
	return (true);
}
