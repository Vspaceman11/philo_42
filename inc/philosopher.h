/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpushkar <vpushkar@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 17:02:46 by vpushkar          #+#    #+#             */
/*   Updated: 2025/09/15 14:39:45 by vpushkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H

# include <sys/time.h>
# include <pthread.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>

typedef struct s_params
{
	long			n;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	long			must_eat_count;
	int				stop;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	stop_mutex;
	struct timeval	start_time;
}	t_params;

typedef struct s_philo
{
	int				philo_id;
	pthread_t		thread;
	t_params		*params;
	long			last_meal_time;
	int				eat_count;
	pthread_mutex_t	meal_mutex;
}	t_philo;


// init.c
int		init_params(t_params *params, int argc, char **argv);
int		init_mutexes(t_params *params);
int		init_philo(t_philo **philos, t_params *params);

// utils.c
int		ft_atol_positive(const char *str, long *out);
int		print_error(const char *msg);
void	cleanup_resources(t_params *params, t_philo *philos);

// philo_simulation.c
int		philo_eat(t_philo *philo);
void	philo_sleep(t_philo *philo);
void	philo_think(t_philo *philo);
void	*philo_routine(void *arg);

// philo_utils.c
void	print_state(t_philo *philo, const char *state);
long	get_timestamp_ms(t_params *params);
void	*monitor_routine(void *arg);
long	get_time(void);
int		is_dead(t_philo *philo);
void	smart_sleep(long ms, t_philo *philo);


#endif
