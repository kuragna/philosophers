/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabourri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 20:46:16 by aabourri          #+#    #+#             */
/*   Updated: 2023/08/23 16:16:43 by aabourri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"


void	philo_pick_fork(int id, int pos, bool *hand, t_data *data)
{
	if (*hand)
		return ;
	pthread_mutex_lock(data->forks + pos);
	*hand = true;
	printf("%ld %d has taken a fork\n", philo_time(data->started_time), id);
}


void	philo_eat(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		philo_pick_fork(philo->id, LEFT, &philo->right_hand, philo->data);
		philo_pick_fork(philo->id, RIGHT, &philo->left_hand, philo->data);
	}
	else
	{
		philo_pick_fork(philo->id, RIGHT, &philo->right_hand, philo->data);
		if (philo->data->number_philos == 1)
		{
			pthread_mutex_unlock(philo->data->forks);
			return ;
		}
		philo_pick_fork(philo->id, LEFT, &philo->left_hand, philo->data);
	}

	if (!philo->right_hand || !philo->left_hand)
		return ;


	printf("%ld %d is eating\n", PHILO_TIME(), philo->id);
	usleep(philo->data->time_to_eat * M_SEC);

	pthread_mutex_lock(philo->data->death_mutex + LEFT);
	philo->last_meal = PHILO_TIME();
	philo->eat_count += 1;
	pthread_mutex_unlock(philo->data->death_mutex + LEFT);

	philo->has_ate = true;
	pthread_mutex_unlock(philo->data->forks + RIGHT);
	philo->right_hand = false;

	pthread_mutex_unlock(philo->data->forks + LEFT);
	philo->left_hand = false;
}

void	philo_sleep_think(t_philo *philo)
{
	if (!philo->has_ate)
		return ;

	printf("%ld %d is sleeping\n", PHILO_TIME(), philo->id);
	usleep(philo->data->time_to_sleep * M_SEC);
	printf("%ld %d is thinking\n", PHILO_TIME(), philo->id);
	usleep(philo->data->time_to_think * M_SEC);
	philo->has_ate = false;
}

//notepme;

int	test = 0;
t_mutex	test_mutex;

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
			pthread_mutex_lock(&test_mutex);
			test += 1;
			pthread_mutex_unlock(&test_mutex);
			return NULL;
		}

		philo_eat(philo);
		philo_sleep_think(philo);
	}
	return (NULL);
}

// void	*philo_death(void *arg)
// {
// 	t_philo *philo = arg;
// 	size_t	count;
// 	size_t	curr_time;
// 	size_t	last_meal;

// 	while (true)
// 	{
// 		pthread_mutex_lock(philo->data->death_mutex + LEFT);
// 		count = philo->eat_count;
// 		if (count == philo->data->meal_number && philo->data->meal_number != 0)
// 		{
// 			pthread_mutex_lock(&philo->data->should_stop_mutex);
// 			philo->should_stop = true;
// 			pthread_mutex_unlock(&philo->data->should_stop_mutex);
// 			return NULL;
// 		}
// 		curr_time = PHILO_TIME();
// 		last_meal = philo->last_meal;
// 		if (curr_time - last_meal >= philo->data->time_to_die)
// 		{
// 			pthread_mutex_unlock(philo->data->death_mutex + LEFT);
// 			printf("%ld %d is died\n", curr_time, philo->id);
// 			pthread_mutex_lock(&philo->data->should_stop_mutex);
// 			philo->should_stop = true;
// 			pthread_mutex_unlock(&philo->data->should_stop_mutex);
// 			return NULL;
// 		}
// 		pthread_mutex_unlock(philo->data->death_mutex + LEFT);
// 	}
// 	return NULL;
// }

void	philo_death(void *arg)
{
	size_t	i;
	t_philo *philo = arg;

	while (true)
	{
		i = 0;
		while (i < philo->data->number_philos)
		{
			pthread_mutex_lock(philo->data->death_mutex + i);
			if (philo->data->meal_number != 0 && philo[i].eat_count == philo->data->meal_number)
			{
				pthread_mutex_lock(&test_mutex);
				if (test == (int)philo->data->meal_number)
				{
					pthread_mutex_unlock(&test_mutex);
					return ;
				}
				pthread_mutex_unlock(&test_mutex);

				pthread_mutex_lock(&philo->data->should_stop_mutex);
				philo[i].should_stop = true;
				pthread_mutex_unlock(&philo->data->should_stop_mutex);
			}
			pthread_mutex_unlock(philo->data->death_mutex + i);
			i += 1;
		}
	}
}

// TODO: stop simulation if a philosopher died or has eaten N number.
// TODO: try to make threads work at the same time.
// TODO: fix time_die for one philosopher.

bool philo_init(t_data *data)
{
	size_t	i;
	int	err;
	t_philo *philo;
// 	pthread_t	threads[data->number_philos];

	philo = malloc(sizeof(*philo) * data->number_philos); // TODO: free memory.
	if (!philo)
		return (false);


	i = 0;
	memset(philo, 0, sizeof(*philo) * data->number_philos);
	pthread_mutex_init(&data->should_stop_mutex, NULL);
	pthread_mutex_init(&test_mutex, NULL);
	while (i < data->number_philos)
	{
		philo[i].id = i + 1;
		philo[i].data = data;
		err = pthread_create(&philo[i].thread, NULL, philo_routine, philo + i);
// 		pthread_detach(philo[i].thread);
		if (err != 0)
		{
			philo_reset_mem(philo);
			return (philo_error(NULL, "Error: Could not create thread\n"));
		}
		i += 1;
	}
	philo_death(philo);
// 	for (size_t j = 0; j < data->number_philos; j++)
// 	{
// 		pthread_create(threads + j, NULL, philo_death, philo + j);
// // 		pthread_detach(threads[j]);
// 	}
// 	for (size_t j = 0; j < data->number_philos; j++)
// 	{
// 		pthread_join(threads[j], NULL);
// 	}
// 	philo_death(philo);
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

// void	philo_death(t_philo *philo)
// {
// 	size_t	i;
// 	int		stop;
// 	size_t	curr_time;
// 	size_t	last_meal;

// 	stop = false;
// 	while (!stop)
// 	{
// 		i = 0;
// 		while (i < philo->data->number_philos)
// 		{
// 			pthread_mutex_lock(philo->data->death_mutex + (philo[i].id - 1));
// // 			if (philo[i].eat_count == philo->data->meal_number && philo->data->meal_number != 0)
// // 			{
// // 				philo[i].data->should_stop = true;
// // 				stop = true;
// // 			}
// // 			curr_time = philo_time(philo->data->started_time);
// 			curr_time = PHILO_TIME();
// 			last_meal = philo[i].last_meal;
// 			if (curr_time - last_meal >= philo->data->time_to_die)
// 			{
// 				pthread_mutex_lock(&philo[i].data->should_stop_mutex);
// 				philo->data->should_stop = true;
// 				pthread_mutex_unlock(&philo[i].data->should_stop_mutex);
// 				printf("%ld %d is died\n", curr_time, philo[i].id);
// 				pthread_mutex_unlock(philo[i].data->death_mutex + (philo[i].id - 1));
// // 				stop = true;
// 				return ;
// 			}
// 			pthread_mutex_unlock(philo[i].data->death_mutex + (philo[i].id - 1));
// 			i += 1;
// 		}
// 	}
// }