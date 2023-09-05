#include "../include/philo_bonus.h"

#include <signal.h>

#define LOG(str) //philo_log(philo, str)

long long	philo_get_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) != 0)
		return (-1);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	philo_usleep(long long time)
{
	const long long	start = philo_get_time() + time;

	while (philo_get_time() < start)
		usleep(100);
}

void	philo_log(t_philo *philo, const char *str)
{
	printf("%lld %d [pid: %d] %s\n", GET_TIME(), philo->id, getpid(), str);
}

void	philo_routine(t_philo *philo)
{
	sem_wait(philo->data->forks);
	LOG("has taken a fork");
// 	philo_log(philo, "has taken a fork");
	sem_wait(philo->data->forks);
	LOG("has taken a fork");
// 	philo_log(philo, "has taken a fork");
// 	philo_log(philo, "is eating");
	LOG("is eating");
	philo_usleep(philo->data->time_to_eat);
	philo->last_meal = GET_TIME();
	philo->meal_number -= 1;
	sem_post(philo->data->forks);
	sem_post(philo->data->forks);
// 	philo_log(philo, "is sleeping");
	LOG("is sleeping");
	philo_usleep(philo->data->time_to_sleep);
	LOG("is thinking");
// 	philo_log(philo, "is thinking");
}


void	*routine(void *arg)
{
	t_philo	*philo = arg;

	if (philo->id % 2 == 0)
		philo_usleep(50);
	while (1)
	{
		philo_routine(philo);
		if (philo->meal_number == 0)
			return NULL;
	}
	return NULL;
}

int	main(int argc, char **argv)
{
	t_data	data;
	data.sem_name = "/philo";

	if (!philo_check_input(argv + 1, argc - 1))
	{
		sem_unlink(data.sem_name);
		return 1;
	}
	
	philo_get_data(&data, argv + 1);
	
	t_philo	*philo = malloc(sizeof(*philo) * data.philo_number); // TODO: check for error

	size_t	i;
	for (i = 0; i < data.philo_number; i++)
	{
		philo[i].id = i + 1;
		philo[i].data = &data;
		philo[i].meal_number = data.meal_number;
	}


	for (i = 0; i < data.philo_number; i++)
	{
		philo[i].pid = fork(); // TODO: check for error
		if (philo[i].pid == 0)
		{
			data.started_time = philo_get_time();
			pthread_create(&philo[i].thread, NULL, routine, philo + i);
			for (;;)
			{
				long long curr_time = (philo_get_time() - philo[i].data->started_time) - philo[i].last_meal;
				if (curr_time >= (long long)philo[i].data->time_to_die)
				{
					printf("here\n");
					exit(0);
				}
// 				printf("i: %ld | id: %d | last meal: %lld\n", i, philo[i].id, philo[i].last_meal);
			}
			pthread_join(philo[i].thread, NULL);
		}
	}

	printf("Done\n");

// 	// wait for child proccess.
	for (i = 0; i < data.philo_number; i++)
	{
		waitpid(philo[i].pid, NULL, 0); // TODO: check for error
	}
	return 0;
}