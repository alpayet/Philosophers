/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trigger_ending.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 20:06:03 by alpayet           #+#    #+#             */
/*   Updated: 2025/08/08 01:05:10 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	abort_check_meal_in_main(size_t philo_nb,
				sem_t *min_meals_count_barrier);
static void	check_philos_meals(size_t philo_nb, sem_t *sem_start_barrier);

void	wait_trigger_ending(long argv_long[5], t_data *data)
{
	if (argv_long[4] != -1)
		check_philos_meals(argv_long[0], data->min_meals_count_barrier.ptr);
	else
		sem_wait(data->sem_end_barrier.ptr);
}

void	trigger_the_end(t_data *data)
{
	if (data->min_meals_count == -1)
		sem_post(data->sem_end_barrier.ptr);
	else
		abort_check_meal_in_main(data->philo_nb,
			data->min_meals_count_barrier.ptr);
}

static void	check_philos_meals(size_t philo_nb, sem_t *min_meals_count_barrier)
{
	size_t	i;

	i = 0;
	while (i < philo_nb)
	{
		sem_wait(min_meals_count_barrier);
		i++;
	}
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
