/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 23:52:36 by alpayet           #+#    #+#             */
/*   Updated: 2025/07/27 23:17:07 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	atol_alt(char *nptr)
{
	int		sign;
	size_t	n;

	if (*nptr == '\0')
		return (-1);
	sign = 1;
	if ((*nptr == '-' || *nptr == '+') && nptr[1] != '\0')
	{
		if (*nptr == '-')
		sign = -sign;
		nptr++;
	}
	n = 0;
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

void	end_the_simulation(t_data *data)
{
	pthread_mutex_lock(&(data->mutex_simulation_end));
	data->end_of_simulation = true;
	pthread_mutex_unlock(&(data->mutex_simulation_end));
}

void	destroy_forks_mutex(size_t fork_nb, t_fork *forks)
{
	size_t	i;

	i = 0;
	while (i < fork_nb)
	{
		pthread_mutex_destroy(&(forks[i].mutex));
		i++;
	}
}

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

void	destroy_one_philo_mutexes(t_philo *philo)
{
	pthread_mutex_destroy(&(philo->mutex_last_time_eat));
	pthread_mutex_destroy(&(philo->mutex_meals_count));
}

void	destroy_philos_mutexes(size_t philo_nb, t_philo *philos)
{
	size_t	i;

	i = 0;
	while (i < philo_nb)
	{
		destroy_one_philo_mutexes(&(philos[i]));
		i++;
	}
}

void cleanup_threads(size_t thread_nb, t_philo *philos)
{
	size_t i;

	i = 0;
	while (i < thread_nb)
	{
		pthread_join(philos[i].thread_id, NULL);
		i++;
	}
}

bool	init_philo_mutexes(t_philo *philo)
{
	if (pthread_mutex_init(&(philo->mutex_last_time_eat), NULL) != 0)
		return (false);
	if (pthread_mutex_init(&(philo->mutex_meals_count), NULL) != 0)
	{
		pthread_mutex_destroy(&(philo->mutex_last_time_eat));
		return (false);
	}
	return (true);
}

t_philo	*cleanup_philos_return_null(size_t philo_nb, t_philo *philos)
{
	destroy_philos_mutexes(philo_nb, philos);
	cleanup_threads(philo_nb, philos);
	free(philos);
	return (NULL);
}

void	init_philo_forks(t_philo *philos, size_t index, t_fork *forks, size_t philo_nb)
{
	philos[index].fork_left = forks + index;
	philos[index].fork_right = forks + (index + 1) % philo_nb;
}

void	init_philo_config(t_philo *philos, size_t index, t_data *data)
{
	philos[index].philo_id = index + 1;
	philos[index].data = data;
	philos[index].meals_count = 0;
}

bool	creat_philo_thread(t_philo *philo)
{
	void	*(*routine)(void *arg);

	if (philo->philo_id % 2 == 0)
		routine = thread_even_routine;
	else
		routine = thread_odd_routine;
	if (pthread_create(&(philo->thread_id), NULL, routine, philo) != 0)
	{
		end_the_simulation(philo->data);
		return (false);
	}
	return (true);
}

t_philo	*create_philos(size_t philo_nb, t_fork *forks, t_data *data)
{
	t_philo *philos;
	size_t i;

	philos = malloc(philo_nb * sizeof(*philos));
	if (check_malloc(philos) == false)
		return (NULL);
	i = 0;
	while (i < philo_nb)
	{
		init_philo_forks(philos, i, forks, philo_nb);
		init_philo_config(philos, i, data);
		if (init_philo_mutexes(&(philos[i])) == false)
		{
			print_error(ERROR_MUTEX_INIT);
			return (cleanup_philos_return_null(i, philos));
		}
		if (creat_philo_thread(&(philos[i])) == false)
		{
			print_error(ERROR_THREAD_CREATE);
			destroy_one_philo_mutexes(&(philos[i]));
			return (cleanup_philos_return_null(i, philos));
		}
		i++;
	}
	return (philos);
}

bool	init_data_mutexes(t_data *data)
{
	if (pthread_mutex_init(&(data->mutex_timestamp), NULL) != 0)
	{
		print_error(ERROR_MUTEX_INIT);
		return (false);
	}
	if (pthread_mutex_init(&(data->mutex_simulation_start), NULL) != 0)
	{
		print_error(ERROR_MUTEX_INIT);
		pthread_mutex_destroy(&(data->mutex_timestamp));
		return (false);
	}
	if (pthread_mutex_init(&(data->mutex_simulation_end), NULL) != 0)
	{
		print_error(ERROR_MUTEX_INIT);
		pthread_mutex_destroy(&(data->mutex_timestamp));
		pthread_mutex_destroy(&(data->mutex_simulation_start));
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
	data->min_meals_count = argv_long[4];
	data->timestamp = 0;
	data->simulation_start_time = -1;
	data->end_of_simulation = false;
	if (init_data_mutexes(data) == false)
		return (false);
	return (true);
}

milliseconds_t	get_current_time_in_ms(void)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	update_timestamp(t_data *data, milliseconds_t simulation_start_time, milliseconds_t current_time)
{
	pthread_mutex_lock(&(data->mutex_timestamp));
	data->timestamp = current_time - simulation_start_time;
	pthread_mutex_unlock(&(data->mutex_timestamp));
}

t_simulation_state	check_philos_last_meal(size_t philo_nb, t_philo *philos, milliseconds_t current_time)
{
	size_t i;

	i = 0;
	while (i < philo_nb)
	{
		pthread_mutex_lock(&(philos[i].mutex_last_time_eat));
		if (current_time - philos[i].last_time_eat >= philos[i].data->time_to_die)
		{
			pthread_mutex_unlock(&(philos[i].mutex_last_time_eat));
			philo_log(&(philos[i]), PHILO_DEAD_MSG);
			end_the_simulation(philos->data);
			return (END_OF_SIMULATION);
		}
		pthread_mutex_unlock(&(philos[i].mutex_last_time_eat));
		i++;
	}
	return (SIMULATION_CONTINUES);
}

t_simulation_state	check_philos_meals_count(size_t philo_nb, t_philo *philos,
	ssize_t min_meals_count)
{
	size_t i;

	i = 0;
	while (i < philo_nb)
	{
		pthread_mutex_lock(&(philos[i].mutex_meals_count));
		if (philos[i].meals_count < min_meals_count)
		{
			pthread_mutex_unlock(&(philos[i].mutex_meals_count));
			break ;
		}
		pthread_mutex_unlock(&(philos[i].mutex_meals_count));
		i++;
	}
	if (i == philo_nb)
	{
		end_the_simulation(philos->data);
		return (END_OF_SIMULATION);
	}
	return (SIMULATION_CONTINUES);
}

void	monitor_philos(size_t philo_nb, t_philo *philos, ssize_t min_meals_count)
{
	milliseconds_t simulation_start_time;
	milliseconds_t current_time;

	pthread_mutex_lock(&(philos->data->mutex_simulation_start));
	simulation_start_time = philos->data->simulation_start_time;
	pthread_mutex_unlock(&(philos->data->mutex_simulation_start));
	while (1)
	{
		current_time = get_current_time_in_ms();
		update_timestamp(philos->data, simulation_start_time, current_time);
		if (check_philos_last_meal(philo_nb,
			philos, current_time) == END_OF_SIMULATION)
			return ;
		if (min_meals_count != -1 && check_philos_meals_count(philo_nb,
				philos, min_meals_count) == END_OF_SIMULATION)
			return ;
		usleep(500);
	}
}

void	full_cleanup(size_t	thread_nb, t_fork *forks, t_data *data, t_philo *philos)
{
	if (philos != NULL)
	{
		cleanup_threads(thread_nb, philos);
		destroy_philos_mutexes(thread_nb, philos);
	}
	if (data != NULL)
	{
		pthread_mutex_destroy(&(data->mutex_timestamp));
		pthread_mutex_destroy(&(data->mutex_simulation_start));
		pthread_mutex_destroy(&(data->mutex_simulation_end));
	}
	if (forks != NULL)
		destroy_forks_mutex(thread_nb, forks);
	free(forks);
	free(philos);
}

bool	argv_to_long(int argc, char **argv, long argv_long[5])
{
	int	i;

	if (argc <= 3 || argc >= 6)
	{
		print_error(ERROR_ARGS_NB);
		return (false);
	}
	if (argc == 4)
		argv_long[4] = -1;
	i = 0;
	while (i < argc)
	{
		argv_long[i] = atol_alt(argv[i]);
		if (argv_long[i] < 0)
		{
			print_error(ERROR_BAD_ARGS);
			return (false);
		}
		i++;
	}
	return (true);
}

void	start_the_simulation(size_t philo_nb, t_philo *philos)
{
	milliseconds_t simulation_start_time;
	size_t	i;

	simulation_start_time = get_current_time_in_ms();
	i = 0;
	while (i < philo_nb)
	{
		philos[i].last_time_eat = simulation_start_time;
		i++;
	}
	pthread_mutex_lock(&(philos->data->mutex_simulation_start));
	philos->data->simulation_start_time = simulation_start_time;
	pthread_mutex_unlock(&(philos->data->mutex_simulation_start));
}

int	main(int argc, char **argv)
{
	long	argv_long[5];
	t_fork	*forks;
	t_data data;
	t_philo *philos;

	if (argv_to_long(argc - 1, argv + 1, argv_long) == false)
		return (EXIT_FAILURE);
	forks = create_forks(argv_long[0]);
	if (forks == NULL)
		return (EXIT_FAILURE);
	if (init_data(&data, argv_long) == false)
	{
		full_cleanup(argv_long[0], forks, NULL, NULL);
		return (EXIT_FAILURE);
	}
	philos = create_philos(argv_long[0], forks, &data);
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
