/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabourri <aabourri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 17:51:39 by aabourri          #+#    #+#             */
/*   Updated: 2023/09/17 17:56:31 by aabourri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <stdio.h>
# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>
# include <string.h>
# include <stdbool.h> 
# include <semaphore.h>
# include <stdlib.h>
# include <fcntl.h>
# include <signal.h>

# define EXIT_MEAL 1
# define EXIT_DIED 0

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
	long long	started_time;
	sem_t		*forks;
	sem_t		*block;
}	t_data;

typedef struct s_philo
{
	int			id;
	size_t		meal_number;
	long long	last_meal;
	pid_t		pid;
	t_data		*data;
}	t_philo;

int			ft_atoi(const char *str);
bool		philo_sem_init(sem_t **sem, const char *sem_name, int sem_size);
bool		philo_get_data(t_data *data, char **args, const int args_size);
bool		philo_error(const char *str);
void		philo_log(t_philo *philo, const char *str);
void		philo_usleep(const long long time, t_philo *philo);
void		philo_destroy(t_data *data, const char *str);
long long	philo_get_time(void);

#endif //PHILO_BONUS_H
