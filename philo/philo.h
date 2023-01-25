/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aechaoub <aechaoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 18:38:16 by aechaoub          #+#    #+#             */
/*   Updated: 2023/01/25 15:17:39 by aechaoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_philo
{
	int				id;
	long			last_meal;
	void			*data;
	void			*left_fork;
	void			*right_fork;
	void			*eating;
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
	pthread_mutex_t	*eat;
	t_philo			*philos;
}					t_data;

int					take_args(int ac, char **av, t_data *data);
long				gettime(void);
void				sleep_well(int time);

#endif