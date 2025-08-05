/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos_meals.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 20:06:03 by alpayet           #+#    #+#             */
/*   Updated: 2025/07/31 16:41:16 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	check_philos_meals(size_t philo_nb, sem_t *sem_start_barrier)
{
	size_t	i;

	i = 0;
	while (i < philo_nb)
	{
		sem_wait(sem_start_barrier);
		i++;
	}
}
