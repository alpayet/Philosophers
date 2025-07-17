/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 23:52:36 by alpayet           #+#    #+#             */
/*   Updated: 2025/07/17 05:38:12 by alpayet          ###   ########.fr       */
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

void	finishing_simulation(t_philo *philos)
{
	pthread_mutex_lock(&(philos->data->mutex_simulation_end));
	philos->data->end_of_simulation = true;
	pthread_mutex_unlock(&(philos->data->mutex_simulation_end));
}

t_fork_state	*create_fork_state_tab(size_t fork_nb)
{
	t_fork_state *fork_state_tab;
	size_t i;

	fork_state_tab = malloc(fork_nb * sizeof(*fork_state_tab));
	if (check_malloc(fork_state_tab) == false)
		return (NULL);
	i = 0;
	while (i < fork_nb)
	{
		fork_state_tab[i] = AVAILABLE;
		i++;
	}
	return (fork_state_tab);
}

void	destroy_mutex_tab(size_t mutex_nb, pthread_mutex_t *mutex_tab)
{
	size_t	i;

	i = 0;
	while (i < mutex_nb)
	{
		pthread_mutex_destroy(mutex_tab + i);
		i++;
	}
}

pthread_mutex_t	*create_fork_mutex_tab(size_t fork_nb)
{
	pthread_mutex_t *fork_mutex_tab;
	size_t i;

	fork_mutex_tab = malloc(fork_nb * sizeof(*fork_mutex_tab));
	if (check_malloc(fork_mutex_tab) == false)
		return (NULL);
	i = 0;
	while (i < fork_nb)
	{
		if (pthread_mutex_init(fork_mutex_tab + i, NULL) != 0)
		{
			print_error(ERROR_MUTEX_INIT);
			destroy_mutex_tab(i, fork_mutex_tab);
			free(fork_mutex_tab);
			return (NULL);
		}
		i++;
	}
	return (fork_mutex_tab);
}

t_fork	*create_fork(t_fork_state *state, pthread_mutex_t *mutex)
{
	t_fork *fork;

	fork = malloc(sizeof(*fork));
	if (fork == NULL)
		return (NULL);
	fork->state = state;
	fork->mutex = mutex;
	return (fork);
}

void	free_ressources(size_t philo_nb, t_fork_state *fork_state_tab,
	pthread_mutex_t *fork_mutex_tab, t_philo *philos)
{
	size_t	i;

	free(fork_state_tab);
	free(fork_mutex_tab);
	if (philos == NULL)
		return ;
	i = 0;
	while (i < philo_nb)
	{
		free(philos[i].fork_left);
		free(philos[i].fork_right);
		i++;
	}
	free(philos);
}

bool	initialize_philo_fork(t_fork **fork, t_fork_state *fork_state, pthread_mutex_t *fork_mutex_tab)
{
	*fork = create_fork(fork_state, fork_mutex_tab);
	if (check_malloc(*fork) == false)
		return (false);
	return (true);
}

bool	initialize_philo_mutexes(t_philo *philo)
{
	if (pthread_mutex_init(&(philo->mutex_last_time_eat), NULL) != 0)
	{
		print_error(ERROR_MUTEX_INIT);
		return (false);
	}
	if (pthread_mutex_init(&(philo->mutex_meals_eaten_nb), NULL) != 0)
	{
		print_error(ERROR_MUTEX_INIT);
		pthread_mutex_destroy(&(philo->mutex_last_time_eat));
		return (false);
	}
	return (true);
}

t_philo	*cleanup_philos_return_null(t_philo *philos, size_t philo_nb)
{
	size_t	i;

	i = 0;
	while (i < philo_nb)
	{
		free(philos[i].fork_left);
		free(philos[i].fork_right);
		pthread_mutex_destroy(&(philos[i].mutex_last_time_eat));
		pthread_mutex_destroy(&(philos[i].mutex_meals_eaten_nb));
		i++;
	}
	free(philos);
	return (NULL);
}

bool	initialize_philo_other_fields(t_philo *philo, size_t philo_id, t_data *data)
{
	struct timeval tv;

	philo->philo_id = philo_id;
	philo->data = data;
	philo->meals_eaten_nb = 0;
	gettimeofday(&tv, NULL);
	philo->last_time_eat = tv.tv_sec * 1000000 + tv.tv_usec;
	return (true);
}

t_philo	*create_philos(size_t philo_nb, t_fork_state *fork_state_tab,
	pthread_mutex_t *fork_mutex_tab, t_data *data)
{
	t_philo *philos;
	size_t i;

	philos = ft_calloc(philo_nb, sizeof(*philos));
	if (check_malloc(philos) == false)
		return (NULL);
	i = 0;
	while (i < philo_nb)
	{
		if (initialize_philo_fork(&(philos[i].fork_left),
			fork_state_tab + i, fork_mutex_tab + i) == false)
			return (cleanup_philos_return_null(philos, i));
		if (initialize_philo_fork(&(philos[i].fork_right),
			fork_state_tab + (i + 1) % philo_nb,
			fork_mutex_tab + (i + 1) % philo_nb) == false)
		{
			free(philos[i].fork_left);
			return (cleanup_philos_return_null(philos, i));
		}
		if (initialize_philo_mutexes(&(philos[i])) == false)
			return (cleanup_philos_return_null(philos, i));
		initialize_philo_other_fields(&(philos[i]), i + 1, data);
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
		if (pthread_create(thread_id_tab + i, NULL, routine, &(philos[i])) != 0)
		{
			finishing_simulation(philos);
			cleanup_threads(i, thread_id_tab);
			free(thread_id_tab);
			return (NULL);
		}
		i++;
	}
	pthread_mutex_lock(&(philos->data->mutex_simulation_start));
	philos->data->start_of_simulation = true;
	pthread_mutex_unlock(&(philos->data->mutex_simulation_start));
	return (thread_id_tab);
}

void	cleanup_mutexes(size_t threads_nb, pthread_mutex_t *fork_mutex_tab,
	t_data *data, t_philo *philos)
{
	size_t	i;

	if (fork_mutex_tab == NULL)
		return ;
	destroy_mutex_tab(threads_nb, fork_mutex_tab);
	if (data == NULL)
		return ;
	pthread_mutex_destroy(&(data->mutex_timestamp));
	pthread_mutex_destroy(&(data->mutex_simulation_start));
	pthread_mutex_destroy(&(data->mutex_simulation_end));
	if (philos == NULL)
		return ;
	i = 0;
	while (i < threads_nb)
	{
		pthread_mutex_destroy(&(philos[i].mutex_last_time_eat));
		pthread_mutex_destroy(&(philos[i].mutex_meals_eaten_nb));
		i++;
	}
}

bool	initialize_data_mutexes(t_data *data)
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
	if (initialize_data_mutexes(data) == false)
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
				finishing_simulation(philos);
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
			finishing_simulation(philos);
			return ;
		}
		usleep(500);
	}
}

int	main(int argc, char **argv)
{
	t_fork_state *fork_state_tab;
	pthread_mutex_t *fork_mutex_tab;
	t_data data;
	t_philo *philos;
	pthread_t *thread_id_tab;

	(void)argc;
	fork_state_tab = create_fork_state_tab(atoi(argv[1]));
	if (fork_state_tab == NULL)
		return (EXIT_FAILURE);
	fork_mutex_tab = create_fork_mutex_tab(atoi(argv[1]));
	if (fork_mutex_tab == NULL)
	{
		free_ressources(atoi(argv[1]), fork_state_tab, NULL, NULL);
		return (EXIT_FAILURE);
	}
	if (init_data(&data, argv) == false)
	{
		cleanup_mutexes(atoi(argv[1]), fork_mutex_tab, NULL, NULL);
		free_ressources(atoi(argv[1]), fork_state_tab, fork_mutex_tab, NULL);
		return (EXIT_FAILURE);
	}
	philos = create_philos(atoi(argv[1]), fork_state_tab, fork_mutex_tab, &data);
	if (philos == NULL)
	{
		cleanup_mutexes(atoi(argv[1]), fork_mutex_tab, &data, NULL);
		free_ressources(atoi(argv[1]), fork_state_tab, fork_mutex_tab, NULL);
		return (EXIT_FAILURE);
	}
	thread_id_tab = creating_threads(atoi(argv[1]), philos);
	if (thread_id_tab == NULL)
	{
		cleanup_mutexes(atoi(argv[1]), fork_mutex_tab, &data, philos);
		free_ressources(atoi(argv[1]), fork_state_tab, fork_mutex_tab, philos);
		return (EXIT_FAILURE);
	}
	monitor_philos(atoi(argv[1]), philos, atoi(argv[5]));
	cleanup_threads(atoi(argv[1]), thread_id_tab);
	free(thread_id_tab);
	cleanup_mutexes(atoi(argv[1]), fork_mutex_tab, &data, philos);
	free_ressources(atoi(argv[1]), fork_state_tab, fork_mutex_tab, philos);
}
