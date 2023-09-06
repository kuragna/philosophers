#include "../include/philo_bonus.h"

#include <signal.h>

#define LOG(str) philo_log(philo, str)

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
	if (philo->data->log)
	{
		printf("%lld %d [pid: %d] %s\n", GET_TIME(), philo->id, getpid(), str);
	}
}

void	philo_routine(t_philo *philo)
{
	sem_wait(philo->data->forks);
	LOG("has taken a fork");
	sem_wait(philo->data->forks);
	LOG("has taken a fork");
	LOG("is eating");
	philo_usleep(philo->data->time_to_eat);
	philo->last_meal = GET_TIME();
	philo->meal_number -= 1;
	sem_post(philo->data->forks);
	sem_post(philo->data->forks);
	LOG("is sleeping");
	philo_usleep(philo->data->time_to_sleep);
	LOG("is thinking");
}


void	*routine(void *arg)
{
	t_philo	*philo = arg;

	while (true)
	{
		if (philo->data->meal_number && philo->meal_number == 0)
			return NULL;
		philo_routine(philo);
	}
	return NULL;
}

int	main(int argc, char **argv)
{
	t_data	data;
	data.sem_name = "/philo";
	data.sem_die_name = "/die";

	if (!philo_check_input(argv + 1, argc - 1))
	{
		sem_unlink(data.sem_name); // TODO: need to remove it
		sem_unlink(data.sem_die_name);
		return (1);
	}

	if (!philo_get_data(&data, argv + 1))
		return (1);


	philo_init(&data);
	
	sem_close(data.forks);
	sem_unlink(data.sem_name);
	return 0;
}

void	philo_thread(t_philo *philo)
{
	int			err;
	long long	die_time;

	err = pthread_create(&philo->thread, NULL, routine, philo);
	if (err != 0)
	{
		philo_error("Error: Could not create thread\n");
		exit(EXIT_FAILURE);
	}

	while (true)
	{
		size_t	last_meal = philo->last_meal;
		die_time = (philo_get_time() - philo->data->started_time) - last_meal;
		if (die_time == (long long)philo->data->time_to_die)
		{
			printf("%lld %d died\n", GET_TIME(), philo->id);
			exit(0);
		}
		if (philo->meal_number == 0 && philo->data->meal_number)
		{
			exit(0);
		}
	}

	err = pthread_join(philo->thread, NULL);
	if (err != 0)
	{
		philo_error("Error: Could not join thread\n");
		exit(EXIT_FAILURE);
	}
}

// TODO: make sure to unlink and close semaphore before kill process manually

bool	philo_init(t_data *data)
{
	t_philo	*philo;
	size_t	i;

	philo = malloc(sizeof(*philo) * data->philo_number);

	if (philo == NULL)
	{
		sem_unlink(data->sem_name); // TODO: check for error
		sem_close(data->forks);
		return (philo_error("Error: Failed to allocate memory\n"));
	}
// 	i = 0;
// 	while (i < data->philo_number)
// 	{
// 		philo[i].id = i + 1;
// 		philo[i].data = data;
// 		i += 1;
// 	}
	i = 0;
	data->started_time = philo_get_time();
	while (i < data->philo_number)
	{
		philo[i].id = i + 1;
		philo[i].data = data;
		philo[i].pid = fork();
		philo[i].meal_number = data->meal_number;
		if (philo[i].pid == -1)
		{
			philo_error("Error: Could not create a process\n");
			sem_unlink(data->sem_name);
			sem_close(data->forks);
			exit(EXIT_FAILURE);
		}
		if (philo[i].pid == 0)
		{
			philo_thread(philo + i);		
		}
		i += 1;
	}
	i = -1;
	while (++i < data->philo_number)
	{
	if (waitpid(philo[i].pid, NULL, 0) == -1)
		{
			philo_error("Error: Failed to wait process\n");
			sem_unlink(data->sem_name);
			sem_close(data->forks);
			exit(EXIT_FAILURE);
		}
	}
	return (true);	
}

// 1 800 200 200
// a philo should die
// 4 310 200 100
// a philo should die
// 4 200 205 200
// a philo should die
// 5 800 200 200 7
// no one should die, simulation should stop after 7 eats
// 4 410 200 200 10
// no one should die, simulation should stop after 10 eats
// -5 600 200 200
// should error and not run (no crashing)
// 4 -5 200 200
// should error and not run (no crashing)
// 4 600 -5 200
// should error and not run (no crashing)
// 4 600 200 -5
// should error and not run (no crashing)
// 4 600 200 200 -5
// should error and not run (no crashing)
//--------------------------------------------
// 5 800 200 200
// no one should die
// 5 600 150 150
// no one should die
// 4 410 200 200
// no one should die
// 100 800 200 200
// no one should die
// 105 800 200 200
// no one should die
// 200 800 200 200
// no one should die