/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aechaoub <aechaoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/01 12:09:15 by aechaoub          #+#    #+#             */
/*   Updated: 2023/01/24 20:12:13 by aechaoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bn_philo.h"

int	ft_isspace(char c)
{
	if (c == ' ' || c == '\n' || c == '\t' || c == '\v'
		|| c == '\r' || c == '\f')
		return (1);
	return (0);
}

int	ft_atoi(const char *str)
{
	int					i;
	int					sn;
	unsigned long int	nbr;

	nbr = 0;
	i = 0;
	sn = 1;
	while (ft_isspace(str[i]))
		i++;
	if (str[i] == '-')
	{
		i++;
		sn *= -1;
	}
	else if (str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		nbr = nbr * 10 + str[i] - '0';
		i++;
	}
	return (nbr * sn);
}

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

int	take_args(int ac, char **av, t_data *data)
{
	int	i;

	if (ac != 6 && ac != 5)
		return (0);
	i = 0;
	data->nbm_philos = atoi(av[1]);
	data->time_to_die = atoi(av[2]);
	data->time_to_eat = atoi(av[3]);
	data->time_to_sleep = atoi(av[4]);
	if (data->time_to_die < 0 || data->time_to_eat < 0
		|| data->time_to_sleep < 0 || data->nbm_philos < 0)
		return (0);
	if (ac == 6)
	{
		data->what_enough = atoi(av[5]);
		if (data->what_enough <= 0)
			return (0);
	}
	return (1);
}
