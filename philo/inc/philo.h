/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 01:38:31 by alpayet           #+#    #+#             */
/*   Updated: 2025/07/04 16:02:41 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdbool.h>
# include <sys/time.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <pthread.h>

# define PHILO_TAKEN_FORK_MSG "has taken a fork\n"
# define PHILO_EATING_MSG "is eating\n"
# define PHILO_SLEEPING_MSG "is sleeping\n"
# define PHILO_THINKING_MSG "is thinking\n"
# define PHILO_DEAD_MSG "died\n"
# define milliseconds long

typedef enum e_return
{
	END_OF_SIMULATION,
	RETURN_SUCESS,
	RETURN_FAILURE
}	t_return;

typedef enum e_philo_state
{
	DEAD,
	ALIVE
}	t_philo_state;

typedef enum e_fork_state
{
	NOT_AVAILABLE,
	AVAILABLE
}	t_fork_state;

typedef struct s_fork
{
	t_fork_state	*state;
	pthread_mutex_t	*mutex;
	size_t	*turn;
}	t_fork;

typedef struct s_data
{
	suseconds_t	time_to_eat;
	suseconds_t	time_to_die;
	suseconds_t	time_to_sleep;
	suseconds_t		timestamp;
	bool			end_of_simulation;
	pthread_mutex_t	mutex_timestamp;
	pthread_mutex_t	mutex_simulation;
}	t_data;

typedef struct s_philo
{
	size_t			philo_id;
	t_data			*data;
	t_fork			*fork_left;
	t_fork			*fork_right;
	bool			can_eat;
	size_t			meals_eaten_nb;
	suseconds_t		remaining_time;
	pthread_mutex_t	mutex_remaining_time;
	pthread_mutex_t	mutex_meals_eaten_nb;

}	t_philo;

t_return	philo_log(t_philo *philo, char *str);
void		*thread_even_routine(void *arg);
void		*thread_odd_routine(void *arg);

#endif
