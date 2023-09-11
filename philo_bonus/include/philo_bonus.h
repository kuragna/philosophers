/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabourri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 17:51:39 by aabourri          #+#    #+#             */
/*   Updated: 2023/09/11 13:21:27 by aabourri         ###   ########.fr       */
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
	long long 	started_time;
	bool		log;
	const char	*sem_name;
	const char	*death_sem_name;
	sem_t		*death;
	sem_t		*forks;
}	t_data;

typedef struct s_philo
{
	int			id;
	bool		should_stop;
	bool		exit_flag;
	bool		log;
	size_t		eat_count;
	size_t		meal_number;
	long long	start_time;
	long long	last_meal;
	pthread_t	thread;
	pid_t		pid; // proccess id
	t_data		*data;
}	t_philo;

// bool	philo_check_input(char **args, const int args_size);
bool	philo_get_data(t_data *data, char **args, const int args_size);
bool	philo_error(const char *str);
int		ft_atoi(const char *str);
bool	philo_init(t_data *data);


#endif //PHILO_BONUS_H
