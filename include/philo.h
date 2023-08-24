/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabourri <aabourri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 21:10:44 by aabourri          #+#    #+#             */
/*   Updated: 2023/08/24 20:20:03 by aabourri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>
# include <string.h>
# include <stdbool.h> 

#define M_SEC 1000
#define LEFT philo->id - 1
#define RIGHT philo->id % philo->data->number_philos
#define PHILO_TIME() philo_time(philo->data->started_time)
#define PHILO_ARG(time) ft_atoi(args[time])

enum e_philo_input
{
	NUMBER_OF_PHILOS = 0,
	T_DIE,
	T_EAT,
	T_SLEEP,
	MEAL_NUMBER,
	T_THINK,
};

typedef pthread_mutex_t	t_mutex;

typedef struct s_data
{
	size_t		number_philos;
	size_t		time_to_eat;
	size_t		time_to_sleep;
	size_t		time_to_think;
	size_t		time_to_die;
	size_t		meal_number;
	size_t		started_time;
	int			meal_flag;
	bool		should_stop;
	bool		log;
	t_mutex		log_mutex;
	t_mutex		meal_mutex;
	t_mutex		should_stop_mutex;
	t_mutex		*death_mutex;
	t_mutex		*forks;
}	t_data;

typedef struct s_philo
{	
	int			id;
	int			should_stop;
	bool		right_hand;
	bool		left_hand;
	bool		has_eaten;
	size_t		eat_count;
	size_t		last_meal;
	pthread_t	thread;
	t_data		*data;
}	t_philo;



int		ft_atoi(const char *str);
int	  	philo_join(t_philo *philos);
int		philo_reset_mem(t_philo *philo);
int		philo_mutex_destroy(t_philo *philo);
int		philo_get_data(t_data *data, char **args, const int n);

size_t	philo_time(size_t started_time);
bool	philo_error(void *ptr, const char *str);

void	philo_eat(t_philo *philo);
void	*philo_routine(void *arg);
void	philo_sleep_think(t_philo *philo);
void	*philo_routine_each_time(void *arg);
// void	philo_pick_fork(int id, int idx, bool *hand, t_data *data);
void	philo_pick_fork(t_philo *philo, const int pos, bool *hand);

t_mutex	*philo_mutex_init(const size_t size);



void	find_leaks(void);

#endif // PHILO_H
