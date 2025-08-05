/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_check.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 20:36:22 by alpayet           #+#    #+#             */
/*   Updated: 2025/08/05 16:05:25 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	abort_check_meal_in_main(size_t philo_nb,
				sem_t *min_meals_count_barrier);

t_sim_state	monitor_simulation(t_philo *philo, char *str)
{
	sem_wait(philo->data->mutex_print_access.ptr);
	sem_wait(philo->mutex_should_exit.ptr);
	if (philo->should_exit == true)
	{
		sem_post(philo->mutex_should_exit.ptr);
		sem_post(philo->data->mutex_print_access.ptr);
		return (END_OF_SIMULATION);
	}
	sem_post(philo->mutex_should_exit.ptr);
	if (get_current_time_in_ms() - philo->last_time_eat
		>= philo->data->time_to_die)
	{
		philo_log(philo, PHILO_DEAD_MSG);
		if (philo->data->min_meals_count == -1)
			sem_post(philo->data->sem_end_barrier.ptr);
		else
			abort_check_meal_in_main(philo->data->philo_nb,
				philo->data->min_meals_count_barrier.ptr);
		return (END_OF_SIMULATION);
	}
	philo_log(philo, str);
	sem_post(philo->data->mutex_print_access.ptr);
	return (SIMULATION_CONTINUES);
}

static void	abort_check_meal_in_main(size_t philo_nb,
				sem_t *min_meals_count_barrier)
{
	size_t	i;

	i = 0;
	while (i < philo_nb)
	{
		sem_post(min_meals_count_barrier);
		i++;
	}
}

t_sim_state	usleep_check(t_philo *philo, t_milliseconds sleep_time)
{
	t_milliseconds	begin_time;
	t_milliseconds	elapsed_time;

	begin_time = get_current_time_in_ms();
	elapsed_time = 0;
	while (elapsed_time < sleep_time)
	{
		if (monitor_simulation(philo, NULL) == END_OF_SIMULATION)
			return (END_OF_SIMULATION);
		elapsed_time = get_current_time_in_ms() - begin_time;
		usleep(500);
	}
	return (SIMULATION_CONTINUES);
}
