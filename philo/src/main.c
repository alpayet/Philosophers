/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 23:52:36 by alpayet           #+#    #+#             */
/*   Updated: 2025/07/16 07:43:37 by alpayet          ###   ########.fr       */
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

void	finishing_simulation(t_philo *philos)
{
	pthread_mutex_lock(&(philos->data->mutex_simulation));
	philos->data->end_of_simulation = true;
	pthread_mutex_unlock(&(philos->data->mutex_simulation));
}

t_fork_state	*create_fork_state_tab(size_t fork_nb)
{
	t_fork_state *fork_state_tab;
	size_t i;

	fork_state_tab = malloc(fork_nb * sizeof(*fork_state_tab));
	if (fork_state_tab == NULL)
	{
		print_error("Error: malloc\n");
		return (NULL);
	}
	i = 0;
	while (i < fork_nb)
	{
		fork_state_tab[i] = AVAILABLE;
		i++;
	}
	return (fork_state_tab);
}

void	destroy_mutexes(size_t mutex_nb, void *mutex_ptr, size_t element_size)
{
	size_t	i;

	i = 0;
	while (i < mutex_nb)
	{
		pthread_mutex_destroy((pthread_mutex_t *)((char *)mutex_ptr + i * element_size));
		i++;
	}
}

pthread_mutex_t	*create_fork_mutex_tab(size_t fork_nb)
{
	pthread_mutex_t *fork_mutex_tab;
	size_t i;

	fork_mutex_tab = malloc(fork_nb * sizeof(*fork_mutex_tab));
	if (fork_mutex_tab == NULL)
	{
		print_error("Error: malloc\n");
		return (NULL);
	}
	i = 0;
	while (i < fork_nb)
	{
		if (pthread_mutex_init(fork_mutex_tab + i, NULL) != 0)
		{
			print_error("Error: mutex_init\n");
			destroy_mutexes(i, fork_mutex_tab, sizeof(*fork_mutex_tab));
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
bool	initialize_philo_fork(t_philo *philo, t_fork_state *fork_state, pthread_mutex_t *fork_mutex_tab, t_fork_side fork_side)
{
	philo->fork_left = create_fork(fork_state, fork_mutex_tab);
	if (philo->fork_left == NULL)
	{
		print_error("Error: malloc\n");
		return (false);
	}
	philo->fork_right = create_fork(fork_state, fork_mutex_tab);
	if (philo->fork_right == NULL)
	{
		print_error("Error: malloc\n");
		return (false);
	}
	return (true);
}

bool	initialize_philo_fork(t_philo *philo, size_t right_index, t_fork_state *fork_state,
	pthread_mutex_t *fork_mutex_tab)
{
	philo->fork_left = create_fork(fork_state, fork_mutex_tab);
	if (philo->fork_left == NULL)
	{
		print_error("Error: malloc\n");
		return (false);
	}
	philo->fork_right = create_fork(fork_state + right_index, fork_mutex_tab + right_index);
	if (philo->fork_right == NULL)
	{
		print_error("Error: malloc\n");
		return (false);
	}
	return (true);
}
bool	initialize_philo_mutexes(t_philo *philos, size_t index)
{
	if (pthread_mutex_init(&(philos[index].mutex_remaining_time), NULL) != 0)
	{
		print_error("Error: mutex_init\n");
		destroy_mutexes(index, &(philos->mutex_remaining_time), sizeof(*philos));
		destroy_mutexes(index, &(philos->mutex_meals_eaten_nb), sizeof(*philos));
		return (false);
	}
	if (pthread_mutex_init(&(philos[index].mutex_meals_eaten_nb), NULL) != 0)
	{
		print_error("Error: mutex_init\n");
		destroy_mutexes(index, &(philos->mutex_remaining_time), sizeof(*philos));
		destroy_mutexes(index, &(philos->mutex_meals_eaten_nb), sizeof(*philos));
		pthread_mutex_destroy(&(philos[index].mutex_remaining_time));
		return (false);
	}
	return (true);
}

t_philo	*create_philos(size_t philo_nb, t_fork_state *fork_state_tab,
	pthread_mutex_t *fork_mutex_tab, t_data *data)
{
	t_philo *philos;
	size_t i;

	philos = ft_calloc(philo_nb, sizeof(*philos));
	if (philos == NULL)
	{
		print_error("Error: malloc\n");
		return (NULL);
	}
	i = 0;
	while (i < philo_nb)
	{
		philos[i].philo_id = i + 1;
		philos[i].data = data;
		philos[i].meals_eaten_nb = 0;
		philos[i].remaining_time = data->time_to_die;
		if (initialize_philo_forks(&(philos[i]), (i + 1) % philo_nb,
			fork_state_tab + i, fork_mutex_tab + i) == false)
			return (NULL);
		if (initialize_philo_mutexes(philos, i) == false)
			return (NULL);
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
	size_t i;
	pthread_t *thread_id_tab;
	void *(*routine)(void *arg);

	thread_id_tab = malloc(threads_nb * sizeof(*thread_id_tab));
	if (thread_id_tab == NULL)
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
	return (thread_id_tab);
}

void cleanup_mutexes(size_t threads_nb, pthread_mutex_t *fork_mutex_tab,
	t_data *data, t_philo *philos)
{
	if (fork_mutex_tab == NULL)
		return ;
	destroy_mutexes(threads_nb, fork_mutex_tab, sizeof(*fork_mutex_tab));
	if (data == NULL)
		return ;
	pthread_mutex_destroy(&(data->mutex_timestamp));
	pthread_mutex_destroy(&(data->mutex_simulation));
	if (philos == NULL)
		return ;
	destroy_mutexes(threads_nb, &(philos->mutex_meals_eaten_nb), sizeof(*philos));
	destroy_mutexes(threads_nb, &(philos->mutex_remaining_time), sizeof(*philos));
}

bool	init_data(t_data *data, char **argv)
{
	data->time_to_die = atoi(argv[2]) * 1000;
	data->time_to_eat = atoi(argv[3]) * 1000;
	data->time_to_sleep = atoi(argv[4]) * 1000;
	data->timestamp = 0;
	data->end_of_simulation = false;
	if (pthread_mutex_init(&(data->mutex_timestamp), NULL) != 0)
	{
		print_error("Error: mutex_init\n");
		pthread_mutex_destroy(&(data->mutex_timestamp));
		return (false);
	}
	if (pthread_mutex_init(&(data->mutex_simulation), NULL) != 0)
	{
		print_error("Error: mutex_init\n");
		pthread_mutex_destroy(&(data->mutex_simulation));
		return (false);
	}
	return (true);
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
				finishing_simulation(philos);
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
		free_ressources(atoi(argv[1]), fork_state_tab, fork_mutex_tab, philos);
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
