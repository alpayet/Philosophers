/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 01:38:31 by alpayet           #+#    #+#             */
/*   Updated: 2025/08/07 23:49:13 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdbool.h>
# include <unistd.h>
# include <stdint.h>
# include <stdlib.h>

# define MAX_THREAD_NB 300
# define ERROR_ARGS_NB "Error: Wrong number of arguments\n"
# define ERROR_THREAD_NB "Error: Wrong number of threads\n"
# define ERROR_BAD_ARGS "Error: bad arguments\n"
# define ERROR_MALLOC "Error: malloc\n"
# define ERROR_THREAD_CREATE "Error: thread_create_init\n"
# define ERROR_MUTEX_INIT "Error: mutex_init\n"

# define PHILO_TAKEN_FORK_MSG "has taken a fork\n"
# define PHILO_EATING_MSG "is eating\n"
# define PHILO_SLEEPING_MSG "is sleeping\n"
# define PHILO_THINKING_MSG "is thinking\n"
# define PHILO_DEAD_MSG "died\n"

//Milliseconds.
typedef int64_t	t_milliseconds;

typedef enum e_return
{
	END_OF_SIMULATION,
	SIMULATION_CONTINUES,
}	t_sim_state;

typedef enum e_fork_state
{
	NOT_AVAILABLE,
	AVAILABLE
}	t_fork_state;

typedef struct s_fork
{
	t_fork_state	state;
	pthread_mutex_t	mutex;
}	t_fork;

typedef struct s_data
{
	size_t			philo_nb;
	t_milliseconds	time_to_eat;
	t_milliseconds	time_to_die;
	t_milliseconds	time_to_sleep;
	ssize_t			min_meals_count;
	t_milliseconds	timestamp;
	t_milliseconds	simulation_start_time;
	bool			end_of_simulation;
	pthread_mutex_t	timestamp_mutex;
	pthread_mutex_t	print_access_mutex;
	pthread_mutex_t	simulation_start_mutex;
	pthread_mutex_t	simulation_end_mutex;
}	t_data;

typedef struct s_philo
{
	t_data			*data;
	size_t			philo_id;
	pthread_t		thread_id;
	t_fork			*fork_left;
	t_fork			*fork_right;
	t_milliseconds	last_time_eat;
	ssize_t			meals_count;
	pthread_mutex_t	mutex_last_time_eat;
	pthread_mutex_t	mutex_meals_count;

}	t_philo;

// MAIN_PROCESS

void			end_the_simulation(t_data *data);
void			full_cleanup(size_t	thread_nb, t_fork *forks,
					t_data *data, t_philo *philos);

// THREADS

void			*thread_even_routine(void *arg);
void			*thread_odd_routine(void *arg);
t_sim_state		philo_log(t_philo *philo, char *str);
bool			is_simulation_ended(t_philo *philo);
t_sim_state		usleep_check(t_philo *philo, t_milliseconds sleep_time);

// UTILS

size_t			ft_strlen(char *str);
long			atol_alt(char *nptr);
bool			print_error(char *str);
bool			check_malloc(void *ptr);
t_milliseconds	get_current_time_in_ms(void);

#endif
