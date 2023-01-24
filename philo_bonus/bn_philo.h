/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bn_philo.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aechaoub <aechaoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 20:07:58 by aechaoub          #+#    #+#             */
/*   Updated: 2023/01/24 20:14:20 by aechaoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BN_PHILO_H
# define BN_PHILO_H

# include <pthread.h>
# include <semaphore.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <sys/types.h>
# include <unistd.h>

typedef struct s_philo
{
	int				id;
	int				pid;
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
	long			time_start;
	pthread_mutex_t	*forks;
	sem_t			*mutex;
	sem_t			*death;
	sem_t			*all_eat;
	t_philo			*philos;
}					t_data;

void	sleep_well(int time);
long	gettime(void);
int		take_args(int ac, char **av, t_data *data);
void	philo_eat(t_philo *philo, t_data *data, int nbrofmeal);
int		init_philos(t_data *data);
#endif