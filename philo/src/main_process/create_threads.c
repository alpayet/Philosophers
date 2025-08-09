/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_threads.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 17:09:04 by alpayet           #+#    #+#             */
/*   Updated: 2025/08/09 17:16:49 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	creat_one_philo_thread(t_philo *philo);

t_philo	*creat_philos_thread(size_t philo_nb, t_philo *philos)
{
	size_t	i;

	i = 0;
	while (i < philo_nb)
	{
		if (creat_one_philo_thread(&(philos[i])) == false)
		{
			full_cleanup(philo_nb, NULL, NULL, philos);
			return (NULL);
		}
		i++;
	}
	return (philos);
}

static bool	creat_one_philo_thread(t_philo *philo)
{
	void	*(*routine)(void *);

	if (philo->philo_id % 2 == 0)
		routine = thread_even_routine;
	else
		routine = thread_odd_routine;
	if (pthread_create(&(philo->thread_id), NULL, routine, philo) != 0)
	{
		end_the_simulation(philo->data);
		return (print_error(ERROR_THREAD_CREATE));
	}
	return (true);
}
