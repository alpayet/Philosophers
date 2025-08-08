/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   secur_sem_open.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 19:49:24 by alpayet           #+#    #+#             */
/*   Updated: 2025/08/08 01:04:04 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <errno.h>
#include <fcntl.h>

static t_named_sem	generate_new_named_sem(char *base, size_t counter);

t_named_sem	open_new_sem_file(char *name, size_t counter)
{
	t_named_sem	sem;

	sem.name = ft_strdup(name);
	if (sem.name == NULL)
		return ((t_named_sem){NULL, NULL, MALLOC});
	errno = 0;
	sem.ptr = sem_open(name, O_CREAT | O_EXCL, 0666, counter);
	if (sem.ptr != SEM_FAILED)
	{
		sem.error = NO_ERROR;
		return (sem);
	}
	free(sem.name);
	if (errno == EEXIST)
		return (generate_new_named_sem(name, counter));
	return ((t_named_sem){SEM_FAILED, NULL, SEM_OPEN});
}

static t_named_sem	generate_new_named_sem(char *base, size_t counter)
{
	sem_t	*new_sem;
	char	*suffix;
	char	*new_name;
	int		i;

	new_name = NULL;
	new_sem = SEM_FAILED;
	i = 0;
	while (new_sem == SEM_FAILED && errno == EEXIST)
	{
		free(new_name);
		suffix = ft_itoa(i);
		if (suffix == NULL)
			return ((t_named_sem){NULL, NULL, MALLOC});
		new_name = ft_strjoin(base, suffix);
		free(suffix);
		if (new_name == NULL)
			return ((t_named_sem){NULL, NULL, MALLOC});
		new_sem = sem_open(new_name, O_CREAT | O_EXCL, 0666, counter);
		i++;
	}
	if (errno != EEXIST)
		return ((t_named_sem){SEM_FAILED, NULL, SEM_OPEN});
	return ((t_named_sem){new_sem, new_name, NO_ERROR});
}
