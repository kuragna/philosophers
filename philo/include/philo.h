/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabourri <aabourri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 21:10:44 by aabourri          #+#    #+#             */
/*   Updated: 2023/09/05 14:03:57 by aabourri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>
# include <string.h>
# include <stdbool.h> 
# include "./philo_mem.h" // manage memory

enum e_philo_input
{
	PHILO_NUMBER = 0,
	T_DIE,
	T_EAT,
	T_SLEEP,
	MEAL_NUMBER,
};

typedef pthread_mutex_t	t_mutex;

typedef struct s_data
{
	size_t		philo_number;
	size_t		time_to_eat;
	size_t		time_to_sleep;
	size_t		time_to_die;
	size_t		started_time;
	size_t		meal_number;
	size_t		meal_flag;
	bool		should_stop;
	bool		log;
	t_mutex		log_mutex;
	t_mutex		eat_mutex;
	t_mutex		stop_mutex;
	t_mutex		meal_mutex;
	t_mutex		*forks;
	t_mem		*mem;
}	t_data;

typedef struct s_philo
{
	int			id;
	bool		should_stop;
	size_t		start_time;
	size_t		eat_count;
	long long	last_meal;
	pthread_t	thread;
	t_data		*data;
}	t_philo;

void		philo_usleep(long long time);

int			ft_atoi(const char *str);

void		philo_log(t_philo *philo, const char *str);

void		philo_death(t_philo *philo);

bool		philo_join(t_philo *philos);

bool		philo_get_data(t_data *data, char **args);

bool		philo_check_input(char **args, const int args_size);

bool		philo_error(const char *str);

void		philo_eat(t_philo *philo);

void		philo_sleep_think(t_philo *philo);

void		find_leaks(void);

bool		philo_mutex_init(t_data *data);

// void		philo_mutex_destroy(t_data *data);
bool		philo_mutex_destroy(t_data *data);

long long	philo_get_time(void);

#endif // PHILO_H
