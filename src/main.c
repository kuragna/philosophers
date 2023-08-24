/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabourri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 20:46:16 by aabourri          #+#    #+#             */
/*   Updated: 2023/08/24 20:35:27 by aabourri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

#define LOG(str) philo_log(philo, str)

void	philo_log(t_philo *philo, const char *str)
{
	int	log;

	pthread_mutex_lock(&philo->data->log_mutex);
	log = philo->data->log;
	pthread_mutex_unlock(&philo->data->log_mutex);

	if (log)
	{
		printf("%ld %d %s\n", PHILO_TIME(), philo->id, str);
	}
}

void	philo_pick_fork(t_philo *philo, const int pos, bool *hand)
{
	if (*hand)
		return ;
	pthread_mutex_lock(philo->data->forks + pos);
	*hand = true;
	LOG("has taken a fork");
// 	philo_log(philo, "has taken a fork");
}

void	philo_eat(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		philo_pick_fork(philo, LEFT, &philo->right_hand);
		philo_pick_fork(philo, RIGHT, &philo->left_hand);
	}
	else
	{
		philo_pick_fork(philo, RIGHT, &philo->right_hand);
		if (philo->data->number_philos == 1)
		{
			pthread_mutex_unlock(philo->data->forks);
			return ;
		}
		philo_pick_fork(philo, LEFT, &philo->left_hand);
	}

	if (!philo->right_hand || !philo->left_hand)
		return ;


// 	philo_log(philo, "is eating");
	LOG("is eating");
	usleep(philo->data->time_to_eat * M_SEC);

	pthread_mutex_lock(philo->data->death_mutex + LEFT);
	philo->last_meal = PHILO_TIME();
	philo->eat_count += 1;
	pthread_mutex_unlock(philo->data->death_mutex + LEFT);

	philo->has_eaten = true;
	pthread_mutex_unlock(philo->data->forks + RIGHT);
	philo->right_hand = false;

	pthread_mutex_unlock(philo->data->forks + LEFT);
	philo->left_hand = false;
}

void	philo_sleep_think(t_philo *philo)
{
	if (!philo->has_eaten)
		return ;

// 	philo_log(philo, "is sleeping");
	LOG("is sleeping");
	usleep(philo->data->time_to_sleep * M_SEC);
// 	philo_log(philo, "is thinking");
	LOG("is thinking");
	usleep(philo->data->time_to_think * M_SEC);
	philo->has_eaten = false;
}

void	*philo_routine(void *arg)
{
	t_philo *philo;
	int	should_stop;


	philo = (t_philo*)arg;
	while (true)
	{
		pthread_mutex_lock(&philo->data->should_stop_mutex);
		should_stop = philo->should_stop;
		pthread_mutex_unlock(&philo->data->should_stop_mutex);

		if (should_stop)
		{
// 			pthread_mutex_lock(&philo->data->meal_mutex);
// 			philo->data->meal_flag += 1;
// 			pthread_mutex_unlock(&philo->data->meal_mutex);
			return (NULL);
		}
		philo_eat(philo);
		philo_sleep_think(philo);
	}
	return (NULL);
}

bool	philo_check_death(t_philo *philo)
{
	size_t	i;
	size_t	last_meal;
	size_t	curr_time;

	i = 0;
	while (i < philo->data->number_philos)
	{
		pthread_mutex_lock(philo->data->death_mutex + i);
		curr_time = PHILO_TIME();
		last_meal = philo[i].last_meal;
		if (curr_time - last_meal >= philo->data->time_to_die)
		{
			philo_log(philo + i, "is died");
			philo[i].data->log = false;
// 			philo[i].should_stop = true;
			for (size_t j = 0; j < philo->data->number_philos; j++)
			{
				philo[j].should_stop = true;
			}
			return (true);
		}
		pthread_mutex_unlock(philo->data->death_mutex + i);
		i += 1;
	}
	return (false);
}

void	philo_death(t_philo *philo)
{
	while (true)
	{
		if (philo_check_death(philo))
			return ;
	}
}


// void	philo_death(void *arg)
// {
// 	size_t	i;
// // 	size_t	count;
// 	size_t	last_meal;
// 	size_t	curr_time;
// 	t_philo *philo = arg;

// 	while (true)
// 	{
// 		i = 0;
// 		while (i < philo->data->number_philos)
// 		{
// 			
// 			curr_time = PHILO_TIME();
// 			last_meal = philo[i].last_meal;

// 			pthread_mutex_lock(philo->data->death_mutex + i);
// 			if (curr_time - last_meal >= philo->data->time_to_die)
// 			{
// // 				pthread_mutex_lock(&philo[i].data->should_stop_mutex);
// 				philo_log(philo, "is died");
// // 				pthread_mutex_lock(&philo[i].data->log_mutex);
// // 				philo[i].data->log = false;
// // 				philo[i].data->should_stop = true;
// // 				pthread_mutex_unlock(&philo[i].data->log_mutex);
// // 				pthread_mutex_unlock(&philo[i].data->should_stop_mutex);
// // 				pthread_mutex_unlock(philo->data->death_mutex + i);
// 				return ;
// 			}
// 			pthread_mutex_unlock(philo->data->death_mutex + i);
// 			i += 1;
// 		}
// 	}
// }

// TODO: stop simulation if a philosopher died or has eaten N number.
// TODO: try to make threads work at the same time.
// TODO: fix time_die for one philosopher.

bool philo_init(t_data *data)
{
	int		err;
	size_t	i;
	t_philo *philo;

	philo = malloc(sizeof(*philo) * data->number_philos); // TODO: free memory.
	if (!philo)
		return (false);


	i = 0;
	pthread_mutex_init(&data->log_mutex, NULL);
	pthread_mutex_init(&data->meal_mutex, NULL);
	pthread_mutex_init(&data->should_stop_mutex, NULL);
	memset(philo, 0, sizeof(*philo) * data->number_philos);

	data->log = true;
	data->should_stop = false;
	while (i < data->number_philos)
	{
		philo[i].id = i + 1;
		philo[i].data = data;
		err = pthread_create(&philo[i].thread, NULL, philo_routine, philo + i);
		if (err != 0)
		{
			philo_reset_mem(philo);
			return (philo_error(NULL, "Error: Could not create thread\n"));
		}
		i += 1;
	}
	philo_death(philo);
	philo_join(philo);
	return (true);
}


int main(int argc, char **argv)
{
	t_data data;

	if (!philo_get_data(&data, argv + 1, argc - 1))
		return (1);
	if (!philo_init(&data))
		return (1);
	return (0);
}
// 			if (philo->data->meal_number != 0 && philo[i].eat_count == philo->data->meal_number)
// 			{
// 				pthread_mutex_lock(&philo->data->should_stop_mutex);
// 				philo[i].should_stop = true;
// 				pthread_mutex_unlock(&philo->data->should_stop_mutex);
// 			}
// 			pthread_mutex_lock(&philo->data->test_mutex);
// 			count = philo[i].data->test;
// 			pthread_mutex_unlock(&philo->data->test_mutex);
// 			if (count == philo->data->number_philos)
// 			{
// 				return ;
// 			}