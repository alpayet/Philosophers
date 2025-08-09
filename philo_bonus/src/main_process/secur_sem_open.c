/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   secur_sem_open.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 19:49:24 by alpayet           #+#    #+#             */
/*   Updated: 2025/08/09 19:56:36 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <errno.h>
#include <fcntl.h>

static bool	try_open_new_sem(char *old_name, int index,
				t_named_sem *new_sem, size_t counter);
static char	*generate_new_name(char *base, int index);

t_named_sem	open_new_sem_file(char *name, size_t counter)
{
	t_named_sem	new_sem;
	int			i;

	new_sem.ptr = SEM_FAILED;
	new_sem.name = NULL;
	errno = 0;
	i = 0;
	while (1)
	{
		if (try_open_new_sem(name, i, &new_sem, counter) == false)
			return ((t_named_sem){SEM_FAILED, NULL});
		if (new_sem.ptr != SEM_FAILED)
			break ;
		i++;
	}
	return (new_sem);
}

static bool	try_open_new_sem(char *old_name, int index,
	t_named_sem *new_sem, size_t counter)
{
	if (new_sem->name == NULL)
		new_sem->name = ft_strdup(old_name);
	if (new_sem->name == NULL)
	{
		print_error(ERROR_MALLOC);
		return (false);
	}
	new_sem->ptr = sem_open(new_sem->name, O_CREAT | O_EXCL, 0666, counter);
	if (new_sem->ptr == SEM_FAILED && errno != EEXIST)
	{
		free(new_sem->name);
		print_error(ERROR_SEM_OPEN);
		return (false);
	}
	if (new_sem->ptr == SEM_FAILED && errno == EEXIST)
	{
		free(new_sem->name);
		new_sem->name = generate_new_name(old_name, index);
		if (new_sem->name == NULL)
		{
			print_error(ERROR_MALLOC);
			return (false);
		}
	}
	return (true);
}

static char	*generate_new_name(char *base, int index)
{
	char	*suffix;
	char	*new_name;

	new_name = NULL;
	suffix = ft_itoa(index);
	if (suffix == NULL)
		return (NULL);
	new_name = ft_strjoin(base, suffix);
	free(suffix);
	if (new_name == NULL)
		return (NULL);
	return (new_name);
}
