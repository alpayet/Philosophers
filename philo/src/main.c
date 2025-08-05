/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 23:52:36 by alpayet           #+#    #+#             */
/*   Updated: 2025/08/05 17:51:51 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	argv_to_long(int argc, char **argv, long argv_long[5]);
t_fork		*create_forks(size_t fork_nb);
bool		init_data(t_data *data, long argv_long[5]);
t_philo		*create_philos(size_t philo_nb, t_fork *forks, t_data *data);
void		start_the_simulation(size_t philo_nb, t_philo *philos);
void		monitor_philos(size_t philo_nb, t_philo *philos,
				ssize_t min_meals_count);

int	main(int argc, char **argv)
{
	long	argv_long[5];
	t_fork	*forks;
	t_data	data;
	t_philo	*philos;

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

static bool	argv_to_long(int argc, char **argv, long argv_long[5])
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
		if (i == 0 && (argv_long[i] == 0 || argv_long[i] > MAX_THREAD_NB))
		{
			print_error(ERROR_THREAD_NB);
			return (false);
		}
		if (argv_long[i] < 0)
		{
			print_error(ERROR_BAD_ARGS);
			return (false);
		}
		i++;
	}
	return (true);
}
