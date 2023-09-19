#include "../include/philo_bonus.h"

long long	philo_get_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) != 0)
		return (-1);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	philo_log(t_philo *philo, const char *str)
{
    const long long curr_time = philo_get_time() - philo->data->started_time;
    
    printf("%lld %d %s\n", curr_time, philo->id, str);
}

void	philo_usleep(long long time, t_philo *philo)
{
	const long long	end = philo_get_time() + time;
    long long   time_comes;

	while (philo_get_time() < end)
	{
        time_comes = philo_get_time() - philo->data->started_time - philo->last_meal;
        if (philo->data->philo_number == 1)
            time_comes = philo->data->time_to_die;
        if (time_comes >= (long long)philo->data->time_to_die)
        {
            sem_wait(philo->data->block);
            printf("%lld %d died\n", time_comes, philo->id);
            exit(EXIT_DIED);
        }
        usleep(100);
    }
}

void    philo_destroy(t_data *data, const char *str)
{
    sem_unlink("forks");
    sem_unlink("block");
    sem_close(data->forks);
    sem_close(data->block);
    philo_error(str);
    exit(EXIT_FAILURE);
}