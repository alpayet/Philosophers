/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_forks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:11:08 by alpayet           #+#    #+#             */
/*   Updated: 2025/08/09 16:42:12 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_fork	*create_forks(size_t fork_nb)
{
	t_fork	*forks;
	size_t	i;

	forks = malloc(fork_nb * sizeof(*forks));
	if (check_malloc(forks) == false)
		return (NULL);
	i = 0;
	while (i < fork_nb)
	{
		forks[i].state = AVAILABLE;
		if (pthread_mutex_init(&(forks[i].mutex), NULL) != 0)
		{
			print_error(ERROR_MUTEX_INIT);
			destroy_forks_mutex(i, forks);
			free(forks);
			return (NULL);
		}
		i++;
	}
	return (forks);
}
