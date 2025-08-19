/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 23:52:36 by alpayet           #+#    #+#             */
/*   Updated: 2025/08/09 22:26:08 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <string.h>

static bool	argv_to_long(int argc, char **argv, long argv_long[5]);
bool		init_data(t_data *data, long argv_long[5]);
t_philo		*create_philos(size_t philo_nb, t_data *data);
void		start_the_simulation(size_t philo_nb, sem_t *sem_start_barrier);
void		wait_trigger_ending(t_data *data);

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
	wait_trigger_ending(&data);
	end_the_simulation(argv_long[0], philos->data);
	if (wait_philos(argv_long[0], philos) == EXIT_FAILURE)
	{
		full_cleanup(argv_long[0], &data, philos);
		return (EXIT_FAILURE);
	}
	full_cleanup(argv_long[0], &data, philos);
	return (EXIT_SUCCESS);
}

static bool	argv_to_long(int argc, char **argv, long argv_long[5])
{
	int	i;

	if (argc <= 3 || argc >= 6)
		return (print_error(ERROR_ARGS_NB));
	if (argc == 4)
		argv_long[4] = -1;
	i = 0;
	while (i < argc)
	{
		argv_long[i] = atol_alt(argv[i]);
		if (i == 0 && (argv_long[i] == 0 || argv_long[i] > MAX_THREAD_NB))
			return (print_error(ERROR_THREAD_NB));
		if (argv_long[i] < 0)
			return (print_error(ERROR_BAD_ARGS));
		i++;
	}
	return (true);
}
