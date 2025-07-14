/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 23:52:36 by alpayet           #+#    #+#             */
/*   Updated: 2025/07/04 18:40:38 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_fork_state	*create_fork_state_tab(size_t fork_nb)
{
	t_fork_state *fork_state_tab;
	size_t i;

	fork_state_tab = malloc(fork_nb * sizeof(*fork_state_tab));
	i = 0;
	while (i < fork_nb)
	{
		fork_state_tab[i] = AVAILABLE;
		i++;
	}
	return (fork_state_tab);
}

pthread_mutex_t	*create_mutexes_tab(size_t fork_nb)
{
	pthread_mutex_t *mutexes_tab;
	size_t i;

	mutexes_tab = malloc(fork_nb * sizeof(*mutexes_tab));
	i = 0;
	while (i < fork_nb)
	{
		pthread_mutex_init(mutexes_tab + i, NULL);
		i++;
	}
	return (mutexes_tab);
}

t_fork	*create_fork(t_fork_state *state, pthread_mutex_t *mutex)
{
	t_fork *fork;

	fork = malloc(sizeof(*fork));
	fork->state = state;
	fork->mutex = mutex;
	return (fork);
}

t_philo	*create_philos(size_t philo_nb, t_fork_state *fork_state_tab,
	pthread_mutex_t *mutexes_tab, t_data *data)
{
	t_philo *philos;
	size_t i;

	philos = malloc(philo_nb * sizeof(*philos));
	i = 0;
	while (i < philo_nb)
	{
		philos[i].philo_id = i + 1;
		philos[i].meals_eaten_nb = 0;
		philos[i].fork_left = create_fork(fork_state_tab + i ,mutexes_tab + i);
		philos[i].fork_right = create_fork(fork_state_tab + (i + 1) % philo_nb,mutexes_tab + (i + 1) % philo_nb);
		philos[i].data = data;
		philos[i].remaining_time = data->time_to_die;
		pthread_mutex_init(&(philos[i].mutex_remaining_time), NULL);
		pthread_mutex_init(&(philos[i].mutex_meals_eaten_nb), NULL);
		i++;
	}
	return (philos);
}

pthread_t	*creating_threads(size_t threads_nb, t_philo *philo)
{
	size_t i;
	pthread_t *thread_id_tab;

	thread_id_tab = malloc(threads_nb * sizeof(*thread_id_tab));
	i = 0;
	while (i < threads_nb)
	{
		if (philo[i].philo_id % 2 == 0)
			pthread_create(thread_id_tab + i, NULL, thread_even_routine, &(philo[i]));
		else
			pthread_create(thread_id_tab + i, NULL, thread_odd_routine, &(philo[i]));
		i++;
	}
	return (thread_id_tab);
}

void cleanup_threads_and_mutexes(size_t threads_nb,
	pthread_t *thread_id_tab, pthread_mutex_t *mutexes_tab)
{
	size_t i;

	i = 0;
	while (i < threads_nb)
	{
		pthread_join(thread_id_tab[i], NULL);
		i++;
	}
	i = 0;
	while (i < threads_nb)
	{
		pthread_mutex_destroy(mutexes_tab + i);
		i++;
	}
}

void	init_data(t_data *data, char **argv)
{
	data->time_to_die = atoi(argv[2]) * 1000;
	data->time_to_eat = atoi(argv[3]) * 1000;
	data->time_to_sleep = atoi(argv[4]) * 1000;
	data->timestamp = 0;
	data->end_of_simulation = false;
	pthread_mutex_init(&(data->mutex_timestamp), NULL);
	pthread_mutex_init(&(data->mutex_simulation), NULL);
}

void	monitor_philos(size_t philo_nb, t_philo *philos, size_t min_meals_eaten_nb)
{
	struct timeval tv;
	suseconds_t begin_of_simulation;
	suseconds_t current_time;
	suseconds_t old_timestamp;
	suseconds_t new_timestamp;
	size_t i;

	gettimeofday(&tv, NULL);
	begin_of_simulation = tv.tv_sec * 1000000 + tv.tv_usec; // a secur
	while (1)
	{
		gettimeofday(&tv, NULL);
		current_time = tv.tv_sec * 1000000 + tv.tv_usec;
		pthread_mutex_lock(&(philos->data->mutex_timestamp));
		old_timestamp = philos->data->timestamp;
		philos->data->timestamp = current_time - begin_of_simulation;
		new_timestamp = philos->data->timestamp;
		pthread_mutex_unlock(&(philos->data->mutex_timestamp));
		if (old_timestamp == 0)
			old_timestamp = new_timestamp;
		i = 0;
		while (i < philo_nb)
		{
			pthread_mutex_lock(&(philos[i].mutex_remaining_time));
			if (philos[i].remaining_time <= (new_timestamp - old_timestamp))
			{
				pthread_mutex_unlock(&(philos[i].mutex_remaining_time));
				philo_log(&(philos[i]), PHILO_DEAD_MSG);
				pthread_mutex_lock(&(philos->data->mutex_simulation));
				philos->data->end_of_simulation = true;
				pthread_mutex_unlock(&(philos->data->mutex_simulation));
				return;
			}
			philos[i].remaining_time -= (new_timestamp - old_timestamp);
			pthread_mutex_unlock(&(philos[i].mutex_remaining_time));
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
			pthread_mutex_lock(&(philos->data->mutex_simulation));
			philos->data->end_of_simulation = true;
			pthread_mutex_unlock(&(philos->data->mutex_simulation));
			return ;
		}
		usleep(500);
	}
}

int	main(int argc, char **argv)
{
	t_fork_state *fork_state_tab;
	pthread_mutex_t *mutexes_tab;
	t_data data;
	t_philo *philos;
	pthread_t *thread_id_tab;

	(void)argc;
	fork_state_tab = create_fork_state_tab(atoi(argv[1]));
	mutexes_tab = create_mutexes_tab(atoi(argv[1]));
	init_data(&data, argv);
	philos = create_philos(atoi(argv[1]), fork_state_tab, mutexes_tab, &data);
	thread_id_tab = creating_threads(atoi(argv[1]), philos);
	monitor_philos(atoi(argv[1]), philos, atoi(argv[5]));
	cleanup_threads_and_mutexes(atoi(argv[1]), thread_id_tab, mutexes_tab);
}