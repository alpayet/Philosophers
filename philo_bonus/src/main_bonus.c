/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 23:52:36 by alpayet           #+#    #+#             */
/*   Updated: 2025/07/24 02:30:29 by alpayet          ###   ########.fr       */
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

sem_t	*create_semaphores(size_t fork_nb)
{
	sem_t	*semaphores;
	sem_t	*sem_forks_nb;
	sem_t	*sem_mutex;

	semaphores = malloc(2 * sizeof(*semaphores));
	sem_mutex = sem_open("/sem_mutex", O_CREAT, ALL_RW, 1);
	if (sem_forks_nb == SEM_FAILED)
	{
		perror("sem_open");
		return (1);
	}
	sem_forks_nb = sem_open("/sem_forks_nb", O_CREAT, ALL_RW, fork_nb);
	if (sem_forks_nb == SEM_FAILED)
	{
		perror("sem_open");
		return (1);
	}
	semaphores[0] = sem_mutex;
	semaphores[1] = sem_forks_nb;
	if (sem_forks_nb == SEM_FAILED)
	{
		perror("sem_open");
		return (1);
	}

}

pid_t	*create_philos(size_t philo_nb)
{
	pid_t	*philos;
	size_t	i;

	philos = malloc(philo_nb * sizeof(*philos));
	while (i < philo_nb)
	{
		philos[i] = fork();
		if (philos[i] == 0)
		{

		}
		if (philos[i] == -1)
		{}
		i++;
	}

}

int	main(int argc, char **argv)
{
	long	argv_long[5];
	sem_t	*sem_forks_nb;
	sem_t	*sem_mutex;
	pid_t	*philos;
	t_data	data;

	if (argv_to_long(argc - 1, argv + 1, argv_long) == false)
		return (EXIT_FAILURE);
	sem_forks_nb = sem_open("/sem_forks_nb", O_CREAT, ALL_RW, argv_long[0]);
	if (sem_forks_nb == SEM_FAILED)
	{
		perror("sem_open");
		return (1);
	}
	if (init_data(&data, argv_long) == false)
	{
		full_cleanup(argv_long[0], forks, NULL, NULL);
		return (EXIT_FAILURE);
	}
	philos = create_philos(argv_long[0]);
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
