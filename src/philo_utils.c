/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpushkar <vpushkar@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 14:43:49 by vpushkar          #+#    #+#             */
/*   Updated: 2025/09/17 12:11:44 by vpushkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"


void	print_state(t_philo *philo, const char *state)
{
	t_params	*params;

	params = philo->params;
	pthread_mutex_lock(&params->print_mutex);
	if (!params->stop)
		printf("%ld %d %s\n", get_timestamp_ms(params),
			philo->philo_id + 1, state);
	pthread_mutex_unlock(&params->print_mutex);
}

long	get_timestamp_ms(t_params *params)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	return ((now.tv_sec - params->start_time.tv_sec) * 1000
		+ (now.tv_usec - params->start_time.tv_usec) / 1000);
}

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int	is_dead(t_philo *philo)
{
	int	dead;

	pthread_mutex_lock(&philo->params->stop_mutex);
	dead = philo->params->stop;
	pthread_mutex_unlock(&philo->params->stop_mutex);
	return (dead);
}

void	smart_sleep(long ms, t_philo *philo)
{
	long	start;

	start = get_time();
	while (!is_dead(philo) && get_time() - start < ms)
		usleep(500);
}
