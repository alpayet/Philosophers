/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 07:58:22 by alpayet           #+#    #+#             */
/*   Updated: 2025/07/23 09:04:04 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int main(void)
{
	sem_t *sem;

	sem = sem_open("/mysem", O_CREAT, ALL_RW, 2);
	if (sem == SEM_FAILED) {
		perror("sem_open");
		return 1;
	}

	printf("Sémaphore ouvert.\n");

	sem_wait(sem);
	//section critique
	sem_post(sem);
	sem_close(sem);
	sem_unlink("/mysem");
}