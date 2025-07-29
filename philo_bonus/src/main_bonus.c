/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 23:52:36 by alpayet           #+#    #+#             */
/*   Updated: 2025/07/28 09:36:17 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
void	full_cleanup(size_t philo_nb, t_data *data, t_philo *philos);

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
	memset(ptr, 0 , nmemb * size);
	return (ptr);
}

void	print_error(char *str)
{
	write(2, str, ft_strlen(str));
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

void	cleanup_semaphore(t_named_sem sem)
{
	sem_close(sem.ptr);
	sem_unlink(sem.name);
	free(sem.name);
}

void	cleanup_data_semaphores(t_data *data)
{
	if (data->mutex_forks_acquisition.ptr != NULL
		&& data->mutex_forks_acquisition.ptr != SEM_FAILED)
		cleanup_semaphore(data->mutex_forks_acquisition);
	if (data->sem_forks_nb.ptr != NULL && data->sem_forks_nb.ptr != SEM_FAILED)
		cleanup_semaphore(data->sem_forks_nb);
	if (data->mutex_print_access.ptr != NULL
		&& data->mutex_print_access.ptr != SEM_FAILED)
		cleanup_semaphore(data->mutex_print_access);
	if (data->sem_start_barrier.ptr != NULL
		&& data->sem_start_barrier.ptr != SEM_FAILED)
		cleanup_semaphore(data->sem_start_barrier);
	if (data->sem_end_barrier.ptr != NULL
		&& data->sem_end_barrier.ptr != SEM_FAILED)
		cleanup_semaphore(data->sem_end_barrier);
}

t_named_sem	generate_new_named_sem(char *base, size_t counter)
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
		new_sem = sem_open(new_name, O_CREAT | O_EXCL, ALL_RW, counter);
		i++;
	}
	if (errno != EEXIST)
		return ((t_named_sem){SEM_FAILED, NULL, SEM_OPEN});
	return ((t_named_sem){new_sem, new_name, NO_ERROR});
}

t_named_sem	open_new_sem_file(char *name, size_t counter)
{
	t_named_sem sem;

	sem.name = ft_strdup(name);
	if (sem.name == NULL)
		return ((t_named_sem){NULL, NULL, MALLOC});
	errno = 0;
	sem.ptr = sem_open(name, O_CREAT | O_EXCL, ALL_RW, counter);
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

t_error_type	open_data_semaphores(t_data *data, size_t fork_nb)
{
	data->mutex_forks_acquisition = open_new_sem_file(
		"/mutex_forks_acquisition", 1);
	if (data->mutex_forks_acquisition.error != NO_ERROR)
		return (data->mutex_forks_acquisition.error);
	data->sem_forks_nb = open_new_sem_file("/sem_forks_nb", fork_nb);
	if (data->sem_forks_nb.error != NO_ERROR)
		return (data->sem_forks_nb.error);
	data->mutex_print_access = open_new_sem_file("/mutex_print_access", 1);
	if (data->mutex_print_access.error != NO_ERROR)
		return (data->mutex_print_access.error);
	data->sem_start_barrier = open_new_sem_file("/sem_start_barrier", 0);
	if (data->sem_start_barrier.error != NO_ERROR)
		return (data->sem_start_barrier.error);
	data->sem_end_barrier = open_new_sem_file("/sem_end_barrier", 0);
	if (data->sem_end_barrier.error != NO_ERROR)
		return (data->sem_end_barrier.error);
	return (NO_ERROR);
}

bool	init_data_semaphores(t_data *data, size_t fork_nb)
{
	t_error_type	error;

	error = open_data_semaphores(data, fork_nb);
	if (error != NO_ERROR)
	{
		cleanup_data_semaphores(data);
		if (error == SEM_OPEN)
			print_error(ERROR_SEM_OPEN);
		if (error == MALLOC)
			print_error(ERROR_MALLOC);
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
	if (init_data_semaphores(data, argv_long[0]) == false)
		return (false);
	return (true);
}

void	start_the_simulation(size_t philo_nb, sem_t *sem_start_barrier)
{
	size_t	i;

	i = 0;
	while (i < philo_nb)
	{
		sem_post(sem_start_barrier);
		i++;
	}
}

void	end_the_simulation(size_t philo_nb, t_philo *philos)
{
	size_t	i;

	i = 0;
	while (i < philo_nb)
	{
		sem_post(philos[i].sem_thread_start_barrier.ptr);
		sem_wait(philos[i].sem_thread_end_barrier.ptr);
		i++;
	}
	sem_post(philos->data->mutex_print_access.ptr);
}

void	cleanup_philo_semaphores(t_philo *philo)
{
	if (philo->mutex_should_exit.ptr != NULL
		&& philo->mutex_should_exit.ptr != SEM_FAILED)
		cleanup_semaphore(philo->mutex_should_exit);
	if (philo->sem_thread_start_barrier.ptr != NULL
		&& philo->sem_thread_start_barrier.ptr != SEM_FAILED)
		cleanup_semaphore(philo->sem_thread_start_barrier);
	if (philo->sem_thread_end_barrier.ptr != NULL
		&& philo->sem_thread_end_barrier.ptr != SEM_FAILED)
		cleanup_semaphore(philo->sem_thread_end_barrier);
}

bool	creat_philo_process(t_philo *philos, size_t index)
{
	pthread_t thread_id;

	philos[index].process_pid = fork();
	if (philos[index].process_pid == 0)
	{
		pthread_create(&thread_id, NULL, thread_routine, &(philos[index]));
		process_routine(&(philos[index]));
		pthread_join(thread_id, NULL);
		cleanup_philo_semaphores(&(philos[index]));
		full_cleanup(index, philos->data, philos);
		exit(EXIT_SUCCESS);
	}
	else if (philos[index].process_pid == -1)
		return (false);
	return (true);
}

t_error_type	open_philo_semaphores(t_philo *philo)
{
	philo->mutex_should_exit = open_new_sem_file(
		"/mutex_should_exit", 1);
	if (philo->mutex_should_exit.error != NO_ERROR)
		return (philo->mutex_should_exit.error);
	philo->sem_thread_start_barrier = open_new_sem_file(
		"/sem_thread_start_barrier", 0);
	if (philo->sem_thread_start_barrier.error != NO_ERROR)
		return (philo->sem_thread_start_barrier.error);
	philo->sem_thread_end_barrier = open_new_sem_file(
		"/sem_thread_end_barrier", 0);
	if (philo->sem_thread_end_barrier.error != NO_ERROR)
		return (philo->sem_thread_end_barrier.error);
	return (NO_ERROR);
}

bool	init_philo_semaphores(t_philo *philo)
{
	t_error_type	error;

	error = open_philo_semaphores(philo);
	if (error != NO_ERROR)
	{
		cleanup_philo_semaphores(philo);
		if (error == SEM_OPEN)
			print_error(ERROR_SEM_OPEN);
		if (error == MALLOC)
			print_error(ERROR_MALLOC);
		return (false);
	}
	return (true);
}

t_philo	*create_philos(size_t philo_nb, t_data *data)
{
	size_t	i;
	t_philo	*philos;

	philos = ft_calloc(philo_nb, sizeof(*philos));
	if (philos == NULL)
		return (NULL);
	data->simulation_start_time = get_current_time_in_ms();
	i = 0;
	while (i < philo_nb)
	{
		philos[i].philo_id = i + 1;
		philos[i].data = data;
		philos[i].meals_count = 0;
		philos[i].should_exit = false;
		if (init_philo_semaphores(&(philos[i])) == false)
		{
			full_cleanup(i, NULL, philos);
			return (NULL);
		}
		if (creat_philo_process(philos, i) == false)
		{
			start_the_simulation(i, data->sem_start_barrier.ptr);
			end_the_simulation(i, philos);
			cleanup_philo_semaphores(&(philos[i]));
			full_cleanup(i, NULL, philos);
			return (NULL);
		}
		i++;
	}
	return (philos);
}

void	full_cleanup(size_t philo_nb, t_data *data, t_philo *philos)
{
	size_t	i;

	if (philos != NULL)
	{
		i = 0;
		while (i < philo_nb)
		{
			waitpid(philos[i].process_pid, NULL, 0);
			cleanup_philo_semaphores(&(philos[i]));
			i++;
		}
	}
	if (data != NULL)
		cleanup_data_semaphores(data);
	free(philos);
}

int	main(int argc, char **argv)
{
	long	argv_long[5];
	t_data	data;
	t_philo	*philos;

	if (argv_to_long(argc - 1, argv + 1, argv_long) == false)
		return (EXIT_FAILURE);
	memset(&data, 0, sizeof(data));
	if (init_data(&data, argv_long) == false)
		return (EXIT_FAILURE);
	philos = create_philos(argv_long[0], &data);
	if (philos == NULL)
	{
		full_cleanup(argv_long[0], &data, NULL);
		return (EXIT_FAILURE);
	}
	start_the_simulation(argv_long[0], data.sem_start_barrier.ptr);
	sem_wait(data.sem_end_barrier.ptr);
	end_the_simulation(argv_long[0], philos);
	full_cleanup(argv_long[0], &data, philos);
	return (EXIT_SUCCESS);
}
