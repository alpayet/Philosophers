/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 01:38:31 by alpayet           #+#    #+#             */
/*   Updated: 2025/07/27 23:17:30 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdbool.h>
# include <sys/time.h>
# include <unistd.h>
# include <string.h>
# include <stdint.h>
# include <limits.h>
# include <stdlib.h>
# include <stdio.h>
# include <pthread.h>

# define ERROR_ARGS_NB "Error: Wrong number of arguments\n"
# define ERROR_BAD_ARGS "Error: bad arguments\n"
# define ERROR_MAX_THREADS "Error: too much threads\n"
# define ERROR_MALLOC "Error: malloc\n"
# define ERROR_MUTEX_INIT "Error: mutex_init\n"
# define ERROR_THREAD_CREATE "Error: thread_create_init\n"

# define PHILO_TAKEN_FORK_MSG "has taken a fork\n"
# define PHILO_EATING_MSG "is eating\n"
# define PHILO_SLEEPING_MSG "is sleeping\n"
# define PHILO_THINKING_MSG "is thinking\n"
# define PHILO_DEAD_MSG "died\n"

//Milliseconds.
typedef int64_t milliseconds_t;

typedef enum e_return
{
	END_OF_SIMULATION,
	SIMULATION_CONTINUES,
}	t_simulation_state;

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
	milliseconds_t	time_to_eat;
	milliseconds_t	time_to_die;
	milliseconds_t	time_to_sleep;
	ssize_t			min_meals_count;
	milliseconds_t	timestamp;
	milliseconds_t	simulation_start_time;
	bool			end_of_simulation;
	pthread_mutex_t	mutex_timestamp;
	pthread_mutex_t	mutex_print_access;
	pthread_mutex_t	mutex_simulation_start;
	pthread_mutex_t	mutex_simulation_end;
}	t_data;

typedef struct s_philo
{
	size_t			philo_id;
	pthread_t 		thread_id;
	t_data			*data;
	t_fork			*fork_left;
	t_fork			*fork_right;
	milliseconds_t	last_time_eat;
	ssize_t			meals_count;
	pthread_mutex_t	mutex_last_time_eat;
	pthread_mutex_t	mutex_meals_count;

}	t_philo;

t_simulation_state	philo_log(t_philo *philo, char *str);
void		*thread_even_routine(void *arg);
void		*thread_odd_routine(void *arg);
milliseconds_t	get_current_time_in_ms(void);

#endif
