/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 23:52:36 by alpayet           #+#    #+#             */
/*   Updated: 2025/07/25 08:52:52 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

long	atol_alt(char *nptr)
{
	int		sign;
	size_t	n;

	n = 0;
	sign = 1;
	if ((*nptr == '-' || *nptr == '+') && nptr[1] != '\0')
	{
		if (*nptr == '-')
			sign = -sign;
		nptr++;
	}
	while (*nptr >= '0' && *nptr <= '9')
	{
		n = 10 * n + *nptr - '0';
		if ((sign == 1 && n > LONG_MAX)
		|| (sign == -1 && n > (size_t)LONG_MAX + 1))
			return (-1);
		nptr++;
	}
	if (*nptr != '\0')
		return (-1);
	return (n);
}

size_t	ft_strlen(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	print_error(char *str)
{
	write(2, str, ft_strlen(str));
}

bool	check_malloc(void *ptr)
{
	if (ptr == NULL)
	{
		print_error(ERROR_MALLOC);
		return (false);
	}
	return (true);
}
milliseconds_t	get_current_time_in_ms(void)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	cleanup_semaphores(t_data *data)
{
	if (data->mutex_forks_acquisition != NULL)
	{
		sem_close(data->mutex_forks_acquisition);
		sem_unlink("/mutex_forks_acquisition");
	}
	if (data->sem_forks_nb != NULL)
	{
		sem_close(data->sem_forks_nb);
		sem_unlink("/sem_forks_nb");
	}
	if (data->sem_blocking != NULL)
	{
		sem_close(data->sem_blocking);
		sem_unlink("/sem_blocking");
	}
	if (data->mutex_simulation_end != NULL)
	{
		sem_close(data->mutex_simulation_end);
		sem_unlink("/mutex_simulation_end");
	}
}

bool	init_data_forks_semaphores(t_data *data, size_t fork_nb)
{
	data->mutex_forks_acquisition = sem_open("/mutex_forks_acquisition",
		O_CREAT, ALL_RW, 1);
	if (data->mutex_forks_acquisition == SEM_FAILED)
	{
		cleanup_semaphores(data);
		return (false);
	}
	data->sem_forks_nb = sem_open("/sem_forks_nb", O_CREAT, ALL_RW, fork_nb);
	if (data->sem_forks_nb  == SEM_FAILED)
	{
		cleanup_semaphores(data);
		return (false);
	}
	return (true);
}

bool	init_data_simulation_semaphores(t_data *data)
{
	data->sem_blocking = sem_open("/sem_blocking",O_CREAT, ALL_RW, 0);
	if (data->sem_blocking == SEM_FAILED)
	{
		cleanup_semaphores(data);
		return (false);
	}
	data->mutex_simulation_end = sem_open("/mutex_simulation_end", O_CREAT, ALL_RW, 1);
	if (data->mutex_simulation_end == SEM_FAILED)
	{
		cleanup_semaphores(data);
		return (false);
	}
	return (true);
}

bool	init_data_semaphores(t_data *data, size_t fork_nb)
{
	sem_t	**semaphores;
	sem_t	*sem_forks_nb;
	sem_t	*sem_mutex;

	if (init_data_forks_semaphores(data, fork_nb) == false
		|| init_data_simulation_semaphores(data) == false)
		{
			print_error(ERROR_SEM_OPEN);
			return (false);
		}
	return (true);
}

bool	init_data(t_data *data, long argv_long[5])
{
	data->philo_nb = argv_long[0];
	data->time_to_die = argv_long[1];
	data->time_to_eat = argv_long[2];
	data->time_to_sleep = argv_long[3];
	data->end_of_simulation = false;
	if (init_data_semaphores(data, argv_long[0]) == false)
		return (false);
	return (true);
}

bool	creat_philo_process(t_philo *philo)
{
	philo->process_pid = fork();
	if (philo->process_pid == 0)
	{
		process_routine(philo);
	}
	else if (philo->process_pid == -1)
		return (false);
}

pid_t	*create_philos(size_t philo_nb, t_data *data)
{
	size_t	i;
	t_philo	*philos;

	philos = malloc(philo_nb * sizeof(*philos));
	data->simulation_start_time = get_current_time_in_ms();
	while (i < philo_nb)
	{
		philos[i].philo_id = i + 1;
		philos[i].data = data;
		philos[i].meals_count = 0;
		if (creat_philo_process(&(philos[i])) == false)
		{
			/////
		}
		i++;
	}
}


void	start_the_simulation(size_t philo_nb, t_philo *philos)
{
	size_t	i;

	i = 0;
	while (i < philo_nb)
	{
		kill(philos[i].process_pid, SIGUSR1);
		i++;
	}
}

int	main(int argc, char **argv)
{
	long	argv_long[5];
	sem_t	**semaphores;
	pid_t	*philos;
	t_data	data;

	if (argv_to_long(argc - 1, argv + 1, argv_long) == false)
		return (EXIT_FAILURE);
	semaphores = create_semaphores(argv_long[0]);
	if (semaphores == NULL)
	{
		full_cleanup(argv_long[0], forks, NULL, NULL);
		return (EXIT_FAILURE);
	}
	memset(&data, 0, sizeof(data));
	if (init_data(&data, argv_long) == false)
	{
		full_cleanup(argv_long[0], forks, NULL, NULL);
		return (EXIT_FAILURE);
	}
	philos = create_philos(argv_long[0], &data);
	if (philos == NULL)
	{
		full_cleanup(argv_long[0], forks, &data, NULL);
		return (EXIT_FAILURE);
	}
	start_the_simulation(argv_long[0], philos);
	monitor_philos(argv_long[0], philos, argv_long[4]);
	full_cleanup(argv_long[0], forks, &data, philos);
	return (EXIT_SUCCESS);
}
