
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

typedef struct s_philo
{
	int				id;
	long			last_meal;
	void			*data;
	void			*left_fork;
	void			*right_fork;
}					t_philo;

typedef struct s_data
{
	int				nbm_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				what_enough;
	int				who_complt;
	long			time_start;
	pthread_mutex_t	*forks;
	t_philo			*philos;
}					t_data;

long	gettime(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	sleep_well(int time)
{
	long	old_time;

	old_time = gettime();
	while (gettime() - old_time < time)
	{
		usleep(500);
	}
}

void	*philo_func(void *arg)
{
	t_philo	*philo;
	t_data	*data;
	int nbrofmeal;

	nbrofmeal=0;

	// t_philo philo = ((t_philo*)*arg);
	philo = (t_philo *)arg;
	data = (t_data *)philo->data;
	if (philo->id % 2 != 0)
		usleep(500);
	while (1)
	{
		pthread_mutex_lock(philo->left_fork);
		printf("%ld %d has taken a fork\n", gettime() - data->time_start,
				philo->id);
		pthread_mutex_lock(philo->right_fork);
		philo->last_meal = gettime();
		printf("%ld %d has taken a fork\n", gettime() - data->time_start,
				philo->id);
		printf("%ld %d is eating\n", gettime() - data->time_start, philo->id);
		sleep_well(data->time_to_eat);
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		nbrofmeal++;
		if(nbrofmeal==data->what_enough)
			data->who_complt++;
		printf("%ld %d is sleeping\n", gettime() - data->time_start, philo->id);
		sleep_well(data->time_to_sleep);
		printf("%ld %d is thinking\n", gettime() - data->time_start, philo->id);
	}
	// printf("Hello, World!\n");
	return (0);
}

int	main(int ac, char **av)
{
	if (ac != 6 && ac != 5)
		return (0);

	t_data data;
	pthread_t thread;

	data.nbm_philos = atoi(av[1]);
	data.time_to_die = atoi(av[2]);
	data.time_to_eat = atoi(av[3]);
	data.time_to_sleep = atoi(av[4]);
	if(data.time_to_die<0 || data.time_to_eat<0 || data.time_to_sleep<0 || data.nbm_philos <0)
			return 0;
	if (ac == 6)
	{
		data.what_enough = atoi(av[5]);
		if(data.what_enough<0)
			return 0; 
	}
	data.philos = malloc(sizeof(t_philo) * data.nbm_philos);

	// pthread_mutex_t *fork;
	data.forks = malloc(sizeof(pthread_mutexattr_t) * data.nbm_philos);

	// pthread_mutexattr_init(&mta[0]);
	for (int i = 0; i < data.nbm_philos; i++)
	{
		pthread_mutex_init(&data.forks[i], NULL);
	}

	for (int i = 0; i < data.nbm_philos; i++)
	{
		data.philos[i].data = &data;
		data.philos[i].id = i;
		data.philos[i].last_meal = gettime();
		data.philos[i].left_fork = &data.forks[i];
		data.philos[i].right_fork = &data.forks[(i + 1) % data.nbm_philos];
	}
	
	data.time_start = gettime();
	for (int i = 0; i < data.nbm_philos; i++)
	{
		pthread_create(&thread, NULL, philo_func, &data.philos[i]);
	}
	// for(int i=0;i<5;i++)
	// {
	//     printf("lol  %d\n",data.philos[i].last_meal);
	//     usleep(5000);
	// }
	while (1)
	{
		for (int i = 0; i < data.nbm_philos; i++)
		{
			if (gettime() - data.philos[i].last_meal > data.time_to_die)
			{
				printf("%ld %d died\n", gettime() - data.time_start, i);
				exit(0);
			}
			if(data.who_complt==data.nbm_philos)
			{
				printf("%ld  Everyone has their meals \n", gettime() - data.time_start);
				exit(0);
			}
		}
		usleep(500);
	}
	// pthread_create(&thread[1],NULL,philo_func,&data.philos[i]);
	pthread_join(thread, NULL);
}