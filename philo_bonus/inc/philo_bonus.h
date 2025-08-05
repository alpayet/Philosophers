/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 08:38:33 by alpayet           #+#    #+#             */
/*   Updated: 2025/08/05 17:54:07 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <pthread.h>
# include <semaphore.h>
# include <stdbool.h>
# include <sys/stat.h>
# include <unistd.h>
# include <stdlib.h>

# define SEM_FILE_FORKS_AQUISITION_MUTEX "/forks_acquisition_mutex"
# define SEM_FILE_FORKS_NB "/forks_nb"
# define SEM_FILE_MIN_MEALS_COUNT_BARRIER "/min_meals_count_barrier"
# define SEM_FILE_PRINT_ACCESS_MUTEX "/print_access_mutex"
# define SEM_FILE_SIMU_START_BARRIER "/simu_start_barrier"
# define SEM_FILE_SIMU_END_BARRIER "/simu_end_barrier"
# define SEM_FILE_SHOULD_EXIT_MUTEX "/should_exit_mutex"
# define SEM_FILE_THREAD_START_BARRIER "/thread_start_barrier"
# define SEM_FILE_THREAD_END_BARRIER "/thread_end_barrier"

# define MAX_THREAD_NB 300
# define ERROR_ARGS_NB "Error: Wrong number of arguments\n"
# define ERROR_THREAD_NB "Error: Wrong number of threads\n"
# define ERROR_BAD_ARGS "Error: bad arguments\n"
# define ERROR_SEM_OPEN "Error : sem_open\n"
# define ERROR_MALLOC "Error: malloc\n"
# define ERROR_THREAD_CREATE "Error: thread_create_init\n"

# define PHILO_TAKEN_FORK_MSG "has taken a fork\n"
# define PHILO_EATING_MSG "is eating\n"
# define PHILO_SLEEPING_MSG "is sleeping\n"
# define PHILO_THINKING_MSG "is thinking\n"
# define PHILO_DEAD_MSG "died\n"

//Milliseconds.
typedef int64_t	t_milliseconds;

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
}	t_sim_state;

typedef struct s_named_sem
{
	sem_t			*ptr;
	char			*name;
	t_error_type	error;
}	t_named_sem;

typedef struct s_data
{
	size_t				philo_nb;
	t_milliseconds		time_to_eat;
	t_milliseconds		time_to_die;
	t_milliseconds		time_to_sleep;
	ssize_t				min_meals_count;
	t_milliseconds		simulation_start_time;
	t_named_sem			sem_forks_nb;
	t_named_sem			mutex_forks_acquisition;
	t_named_sem			min_meals_count_barrier;
	t_named_sem			mutex_print_access;
	t_named_sem			sem_start_barrier;
	t_named_sem			sem_end_barrier;
}	t_data;

typedef struct s_philo
{
	size_t				philo_id;
	pid_t				process_pid;
	t_data				*data;
	t_milliseconds		last_time_eat;
	ssize_t				meals_count;
	bool				should_exit;
	t_named_sem			mutex_should_exit;
	t_named_sem			sem_thread_start_barrier;
	t_named_sem			sem_thread_end_barrier;
}	t_philo;

//MAIN_PROCESS

t_named_sem		open_new_sem_file(char *name, size_t counter);
void			start_the_simulation(size_t philo_nb, sem_t *sem_start_barrier);
void			end_the_simulation(size_t philo_nb, t_philo *philos);
void			full_cleanup(size_t philo_nb, t_data *data, t_philo *philos);

// CHILDS_PROCESS

void			*thread_routine(void *arg);
void			process_routine(t_philo *philo);
t_sim_state		usleep_check(t_philo *philo, t_milliseconds sleep_time);
t_sim_state		monitor_simulation(t_philo *philo, char *str);
void			philo_log(t_philo *philo, char *str);

// UTILS

long			atol_alt(char *nptr);
size_t			ft_strlen(char *str);
char			*ft_strdup(char *s);
char			*ft_itoa(int n);
void			*ft_calloc(size_t nmemb, size_t size);
char			*ft_strjoin(char *s1, char *s2);
void			print_error(char *str);
bool			check_malloc(void *ptr);
t_milliseconds	get_current_time_in_ms(void);

#endif