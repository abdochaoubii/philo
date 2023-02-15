/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aechaoub <aechaoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 19:39:36 by aechaoub          #+#    #+#             */
/*   Updated: 2023/02/03 18:54:00 by aechaoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bn_philo.h"

void	*confirm_death(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)arg;
	sem_wait(data->death);
	i = 0;
	while (i < data->nbm_philos)
	{
		sem_post(data->all_eat);
		i++;
	}
	return (0);
}

void	*checkdeath(void *arg)
{
	t_philo	*philo;
	t_data	*data;


	philo = (t_philo *)arg;
	data = (t_data *)philo->data;
	while (1)
	{
		sem_wait(philo->print);
		if (gettime() - philo->last_meal > data->time_to_die)
		{
			printf("%ld %d died\n", gettime() - data->time_start, philo->id);
			sem_post(data->death);
			break ;
		}
		sem_post(philo->print);
		usleep(500);
	}
	return (0);
}

void	*philo_func(void *arg)
{
	t_philo		*philo;
	t_data		*data;
	int			nbrofmeal;
	pthread_t	thread;

	nbrofmeal = 0;
	philo = (t_philo *)arg;
	data = (t_data *)philo->data;
	philo->print = sem_open("/print", O_CREAT | O_RDWR, 0644, 1);
	sem_wait(data->death);
	sem_wait(data->all_eat);
	pthread_create(&thread, NULL, checkdeath, arg);
	pthread_detach(thread);
	while (1)
	{
		nbrofmeal++;
		philo_eat(philo, data, nbrofmeal);
		sem_wait(philo->print);
		printf("%ld %d is sleeping\n", gettime() - data->time_start, philo->id);
		sem_post(philo->print);
		sleep_well(data->time_to_sleep);
		sem_wait(philo->print);
		printf("%ld %d is thinking\n", gettime() - data->time_start, philo->id);
		sem_post(philo->print);
	}
	return (0);
}

int	check_death(t_data *data)
{
	pthread_t	thread;
	int			i;

	pthread_create(&thread, NULL, confirm_death, data);
	pthread_detach(thread);
	i = 0;
	while (i < data->nbm_philos)
	{
		sem_wait(data->all_eat);
		i++;
	}
	i = 0;
	while (i < data->nbm_philos)
	{
		kill(data->philos[i].pid, 2);
		i++;
	}
	return (0);
}

int	main(int ac, char **av)
{
	t_data		data;
	pid_t		child_pid;
	int			i;

	if (!take_args(ac, av, &data))
		return (0);
	sem_unlink("/fork");
	sem_unlink("/death");
	sem_unlink("/all_eat");
	sem_unlink("/print");
	if (!init_philos(&data))
		return (0);
	i = 0;
	while (i < data.nbm_philos)
	{
		child_pid = fork();
		if (child_pid == 0)
		{
			philo_func(&data.philos[i++]);
			return (0);
		}
		else
			data.philos[i].pid = child_pid;
		i++;
	}
	usleep(200);
	check_death(&data);
}
