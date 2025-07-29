/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 08:38:33 by alpayet           #+#    #+#             */
/*   Updated: 2025/07/28 09:35:51 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <errno.h>
# include <stdbool.h>
# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>
# include <string.h>
# include <stdint.h>
# include <limits.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <semaphore.h>
# include <signal.h>

# define ALL_RW (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)

# define ERROR_SEM_OPEN "Error : sem_open\n"
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

typedef enum e_error_type
{
	NO_ERROR,
	MALLOC,
	SEM_OPEN,
}	t_error_type;

typedef enum e_simulation_state
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
	t_error_type	error;
}	t_fork;

typedef struct s_named_sem
{
	sem_t	*ptr;
	char	*name;
	t_error_type	error;
}	t_named_sem;

typedef struct s_data
{
	size_t				philo_nb;
	milliseconds_t		time_to_eat;
	milliseconds_t		time_to_die;
	milliseconds_t		time_to_sleep;
	ssize_t				min_meals_count;
	milliseconds_t		simulation_start_time;
	t_named_sem	sem_forks_nb;
	t_named_sem	mutex_forks_acquisition;
	t_named_sem	mutex_print_access;
	t_named_sem	sem_start_barrier;
	t_named_sem	sem_end_barrier;
}	t_data;

typedef struct s_philo
{
	size_t				philo_id;
	pid_t				process_pid;
	t_data				*data;
	milliseconds_t		last_time_eat;
	ssize_t				meals_count;
	bool				should_exit;
	t_named_sem			mutex_should_exit;
	t_named_sem			sem_thread_start_barrier;
	t_named_sem			sem_thread_end_barrier;
}	t_philo;

size_t			ft_strlen(char *str);
char			*ft_strdup(char *s);
char			*ft_itoa(int n);
char			*ft_strjoin(char *s1, char *s2);
void			*thread_routine(void *arg);
void			process_routine(t_philo *philo);
milliseconds_t	get_current_time_in_ms(void);
void	philo_log(t_philo *philo, char *str);

#endif