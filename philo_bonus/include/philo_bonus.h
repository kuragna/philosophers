/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabourri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 17:51:39 by aabourri          #+#    #+#             */
/*   Updated: 2023/09/06 20:11:05 by aabourri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
#define PHILO_BONUS_H

# include <stdio.h>
# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>
# include <string.h>
# include <stdbool.h> 
# include <semaphore.h>
# include <stdlib.h>

#define GET_TIME() philo_get_time() - philo->data->started_time

enum	e_philo_input
{
	PHILO_NUMBER = 0,
	T_DIE,
	T_EAT,
	T_SLEEP,
	MEAL_NUMBER,
};

typedef struct s_data
{
	size_t		philo_number;
	size_t		time_to_eat;
	size_t		time_to_sleep;
	size_t		time_to_die;
	size_t		meal_number;
	size_t		started_time;
	bool		log;
	const char	*sem_die_name;
	const char	*sem_name;
	sem_t		*die;
	sem_t		*forks;
}	t_data;

typedef struct s_philo
{
	int			id;
	bool		should_stop;
	size_t		eat_count;
	size_t		meal_number;
	size_t		start_time;
	_Atomic long long	last_meal; // NOTE: maybe we can't use it
	pthread_t	thread;
	pid_t		pid; // proccess id
	t_data		*data;
}	t_philo;

bool	philo_check_input(char **args, const int args_size);
bool	philo_get_data(t_data *data, char **args);
bool	philo_error(const char *str);
int		ft_atoi(const char *str);
void	philo_thread(t_philo *philo);
bool	philo_init(t_data *data);


#endif //PHILO_BONUS_H
