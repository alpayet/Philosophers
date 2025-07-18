/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 23:52:36 by alpayet           #+#    #+#             */
/*   Updated: 2025/07/18 09:59:38 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t	ft_strlen(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*ptr;
	if (nmemb == 0 || size == 0)
		return (malloc(0));
	if (nmemb > (SIZE_MAX / size))
		return (NULL);
	ptr = malloc(nmemb * size);
	if (ptr == NULL)
		return (NULL);
	memset(ptr, 0, nmemb * size);
	return (ptr);
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

void	finishing_simulation(t_data *data)
{
	pthread_mutex_lock(&(data->mutex_simulation_end));
	data->end_of_simulation = true;
	pthread_mutex_unlock(&(data->mutex_simulation_end));
}

void	destroy_mutexes_philos(size_t philo_nb, t_philo *philos)
{
	size_t	i;

	i = 0;
	while (i < philo_nb)
	{
		pthread_mutex_destroy(&(philos[i].mutex_last_time_eat));
		pthread_mutex_destroy(&(philos[i].mutex_meals_eaten_nb));
		i++;
	}
}

void	destroy_mutex_forks(size_t fork_nb, t_fork *forks)
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
			destroy_mutex_forks(i, forks);
			free(forks);
			return (NULL);
		}
		i++;
	}
	return (forks);
}

bool	init_philo_mutexes(t_philo *philo)
{
	if (pthread_mutex_init(&(philo->mutex_last_time_eat), NULL) != 0)
		return (false);
	if (pthread_mutex_init(&(philo->mutex_meals_eaten_nb), NULL) != 0)
	{
		pthread_mutex_destroy(&(philo->mutex_last_time_eat));
		return (false);
	}
	return (true);
}

t_philo	*cleanup_philos_return_null(size_t philo_nb, t_philo *philos)
{
	destroy_mutexes_philos(philo_nb, philos);
	free(philos);
	return (NULL);
}

bool	init_philo_other_fields(t_philo *philos, size_t index, t_data *data)
{
	struct timeval tv;

	philos[index].philo_id = index + 1;
	philos[index].data = data;
	philos[index].meals_eaten_nb = 0;
	gettimeofday(&tv, NULL);
	philos[index].last_time_eat = tv.tv_sec * 1000000 + tv.tv_usec;
	return (true);
}

t_philo	*create_philos(size_t philo_nb, t_fork *forks, t_data *data)
{
	t_philo *philos;
	size_t i;

	philos = ft_calloc(philo_nb, sizeof(*philos));
	if (check_malloc(philos) == false)
		return (NULL);
	i = 0;
	while (i < philo_nb)
	{
		philos[i].fork_left = forks + i;
		philos[i].fork_right = forks + (i + 1) % philo_nb;
		if (init_philo_mutexes(&(philos[i])) == false)
		{
			print_error(ERROR_MUTEX_INIT);
			return (cleanup_philos_return_null(i, philos));
		}
		init_philo_other_fields(philos, i, data);
		i++;
	}
	return (philos);
}

void cleanup_threads(size_t threads_nb, pthread_t *thread_id_tab)
{
	size_t i;

	i = 0;
	while (i < threads_nb)
	{
		pthread_join(thread_id_tab[i], NULL);
		i++;
	}
}

bool	secur_thread_create(pthread_t *thread_id_tab, size_t index,
	void *(*routine)(void *), t_philo *philo)
{
	if (index == NB_MAX_THREADS
		|| pthread_create(thread_id_tab + index, NULL, routine, philo) != 0)
	{
		print_error(ERROR_THREAD_CREATE);
		finishing_simulation(philo->data);
		cleanup_threads(index, thread_id_tab);
		free(thread_id_tab);
		return (false);
	}
	return (true);
}

pthread_t	*creating_threads(size_t threads_nb, t_philo *philos)
{
	size_t		i;
	pthread_t	*thread_id_tab;
	void 		*(*routine)(void *arg);

	thread_id_tab = malloc(threads_nb * sizeof(*thread_id_tab));
	if (check_malloc(thread_id_tab) == false)
		return (NULL);
	i = 0;
	while (i < threads_nb)
	{
		if (philos[i].philo_id % 2 == 0)
			routine = thread_even_routine;
		else
			routine = thread_odd_routine;
		if (secur_thread_create(thread_id_tab, i, routine, &(philos[i])) == false)
			return (NULL);
		i++;
	}
	pthread_mutex_lock(&(philos->data->mutex_simulation_start));
	philos->data->start_of_simulation = true;
	pthread_mutex_unlock(&(philos->data->mutex_simulation_start));
	return (thread_id_tab);
}

void	cleanup_mutexes(size_t threads_nb, t_fork *forks,
	t_data *data, t_philo *philos)
{
	if (forks == NULL)
		return ;
	destroy_mutex_forks(threads_nb, forks);
	if (data == NULL)
		return ;
	pthread_mutex_destroy(&(data->mutex_timestamp));
	pthread_mutex_destroy(&(data->mutex_simulation_start));
	pthread_mutex_destroy(&(data->mutex_simulation_end));
	if (philos == NULL)
		return ;
	destroy_mutexes_philos(threads_nb, philos);
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

bool	init_data(t_data *data, char **argv)
{
	data->time_to_die = atoi(argv[2]) * 1000;
	data->time_to_eat = atoi(argv[3]) * 1000;
	data->time_to_sleep = atoi(argv[4]) * 1000;
	data->timestamp = 0;
	data->start_of_simulation = false;
	data->end_of_simulation = false;
	if (init_data_mutexes(data) == false)
		return (false);
	return (true);
}

void	monitor_philos(size_t philo_nb, t_philo *philos, size_t min_meals_eaten_nb)
{
	struct timeval tv;
	suseconds_t simulation_start_time;
	suseconds_t current_time;
	size_t i;

	gettimeofday(&tv, NULL);
	simulation_start_time = tv.tv_sec * 1000000 + tv.tv_usec; // a secur
	while (1)
	{
		gettimeofday(&tv, NULL);
		current_time = tv.tv_sec * 1000000 + tv.tv_usec;
		pthread_mutex_lock(&(philos->data->mutex_timestamp));
		philos->data->timestamp = current_time - simulation_start_time;
		pthread_mutex_unlock(&(philos->data->mutex_timestamp));
		i = 0;
		while (i < philo_nb)
		{
			pthread_mutex_lock(&(philos[i].mutex_last_time_eat));
			if (current_time - philos[i].last_time_eat >= philos->data->time_to_die)
			{
				pthread_mutex_unlock(&(philos[i].mutex_last_time_eat));
				philo_log(&(philos[i]), PHILO_DEAD_MSG);
				finishing_simulation(philos->data);
				return;
			}
			pthread_mutex_unlock(&(philos[i].mutex_last_time_eat));
			i++;
		}
		i = 0;
		while (i < philo_nb)
		{
			pthread_mutex_lock(&(philos[i].mutex_meals_eaten_nb));
			if (philos[i].meals_eaten_nb < min_meals_eaten_nb)
			{
				pthread_mutex_unlock(&(philos[i].mutex_meals_eaten_nb));
				break ;
			}
			pthread_mutex_unlock(&(philos[i].mutex_meals_eaten_nb));
			i++;
		}
		if (i == philo_nb)
		{
			finishing_simulation(philos->data);
			return ;
		}
		usleep(500);
	}
}

int	full_cleanup(size_t	thread_nb, t_fork *forks, t_data *data, t_philo *philos)
{
	cleanup_mutexes(thread_nb, forks, data, philos);
	free(forks);
	free(philos);
	return (EXIT_FAILURE);
}

int	main(int argc, char **argv)
{
	t_fork	*forks;
	t_data data;
	t_philo *philos;
	pthread_t *thread_id_tab;

	(void)argc;
	forks = create_forks(atoi(argv[1]));
	if (forks == NULL)
		return (EXIT_FAILURE);
	if (init_data(&data, argv) == false)
		return (full_cleanup(atoi(argv[1]), forks, NULL, NULL));
	philos = create_philos(atoi(argv[1]), forks, &data);
	if (philos == NULL)
		return (full_cleanup(atoi(argv[1]), forks, &data, NULL));
	thread_id_tab = creating_threads(atoi(argv[1]), philos);
	if (thread_id_tab == NULL)
		return (full_cleanup(atoi(argv[1]), forks, &data, philos));
	monitor_philos(atoi(argv[1]), philos, atoi(argv[5]));
	cleanup_threads(atoi(argv[1]), thread_id_tab);
	free(thread_id_tab);
	full_cleanup(atoi(argv[1]), forks, &data, philos);
	return (EXIT_SUCCESS);
}
