/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpushkar <vpushkar@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 17:02:46 by vpushkar          #+#    #+#             */
/*   Updated: 2025/09/09 17:14:05 by vpushkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H

# include <sys/time.h>
# include <pthread.h>
# include <stdlib.h>
# include <stdio.h>

typedef struct s_params
{
	int				n;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				must_eat_count;
	struct timeval	start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	stop_mutex;
	int				stop;
}	t_params;

typedef struct s_philo
{
	int				philo_id;
	pthread_t		thread;
	long			last_meal_ms;
	int				eat_count;
	pthread_mutex_t	meal_mutex;
	t_params		*params;
}	t_philo;

#endif
