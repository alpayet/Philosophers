/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 11:53:28 by alpayet           #+#    #+#             */
/*   Updated: 2025/08/05 16:44:01 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void			update_timestamp(t_data *data,
						t_milliseconds simulation_start_time,
						t_milliseconds current_time);
static t_sim_state	check_philos_last_meal(size_t philo_nb,
						t_philo *philos, t_milliseconds current_time);
static t_sim_state	check_philos_meals_count(size_t philo_nb,
						t_philo *philos, ssize_t min_meals_count);

void	monitor_philos(size_t philo_nb, t_philo *philos,
	ssize_t min_meals_count)
{
	t_milliseconds	simulation_start_time;
	t_milliseconds	current_time;

	pthread_mutex_lock(&(philos->data->mutex_simulation_start));
	simulation_start_time = philos->data->simulation_start_time;
	pthread_mutex_unlock(&(philos->data->mutex_simulation_start));
	while (1)
	{
		current_time = get_current_time_in_ms();
		update_timestamp(philos->data, simulation_start_time, current_time);
		if (check_philos_last_meal(philo_nb,
				philos, current_time) == END_OF_SIMULATION)
			return ;
		if (min_meals_count != -1 && check_philos_meals_count(philo_nb,
				philos, min_meals_count) == END_OF_SIMULATION)
			return ;
		usleep(500);
	}
}

static void	update_timestamp(t_data *data, t_milliseconds simulation_start_time,
	t_milliseconds current_time)
{
	pthread_mutex_lock(&(data->mutex_timestamp));
	data->timestamp = current_time - simulation_start_time;
	pthread_mutex_unlock(&(data->mutex_timestamp));
}

static t_sim_state	check_philos_last_meal(size_t philo_nb,
	t_philo *philos, t_milliseconds current_time)
{
	size_t	i;

	i = 0;
	while (i < philo_nb)
	{
		pthread_mutex_lock(&(philos[i].mutex_last_time_eat));
		if (current_time - philos[i].last_time_eat
			>= philos[i].data->time_to_die)
		{
			pthread_mutex_unlock(&(philos[i].mutex_last_time_eat));
			philo_log(&(philos[i]), PHILO_DEAD_MSG);
			end_the_simulation(philos->data);
			return (END_OF_SIMULATION);
		}
		pthread_mutex_unlock(&(philos[i].mutex_last_time_eat));
		i++;
	}
	return (SIMULATION_CONTINUES);
}

static t_sim_state	check_philos_meals_count(size_t philo_nb,
	t_philo *philos, ssize_t min_meals_count)
{
	size_t	i;

	i = 0;
	while (i < philo_nb)
	{
		pthread_mutex_lock(&(philos[i].mutex_meals_count));
		if (philos[i].meals_count < min_meals_count)
		{
			pthread_mutex_unlock(&(philos[i].mutex_meals_count));
			break ;
		}
		pthread_mutex_unlock(&(philos[i].mutex_meals_count));
		i++;
	}
	if (i == philo_nb)
	{
		end_the_simulation(philos->data);
		return (END_OF_SIMULATION);
	}
	return (SIMULATION_CONTINUES);
}
