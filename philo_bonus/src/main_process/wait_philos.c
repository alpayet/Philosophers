/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_philos.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 22:40:43 by alpayet           #+#    #+#             */
/*   Updated: 2025/08/08 23:04:07 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <sys/types.h>
#include <sys/wait.h>

int	wait_philos(size_t philo_nb, t_philo *philos)
{
	int		wstatus;
	int		exit_code;
	size_t	i;

	exit_code = EXIT_SUCCESS;
	i = 0;
	while (i < philo_nb)
	{
		if (waitpid(philos[i].process_pid, &wstatus, 0) != -1)
		{
			if (exit_code == EXIT_SUCCESS && WIFEXITED(wstatus) == true)
			{
				if (WEXITSTATUS(wstatus) == EXIT_FAILURE)
				{
					exit_code = EXIT_FAILURE;
				}
			}
		}
		i++;
	}
	return (exit_code);
}
