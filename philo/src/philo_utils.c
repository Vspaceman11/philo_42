/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpushkar <vpushkar@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 14:43:49 by vpushkar          #+#    #+#             */
/*   Updated: 2025/09/23 14:08:08 by vpushkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

/**
 * @brief Prints the current state of a philosopher.
 *
 * Locks the print_mutex to safely output the philosopher's state. Will not
 * print if the simulation has been stopped.
 *
 * @param philo Pointer to the philosopher structure (t_philo *).
 * @param state String describing the current state of the philosopher.
 */
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

/**
 * @brief Returns the elapsed time in milliseconds since the simulation start.
 *
 * Calculates the difference between the current time and the start_time in
 * the t_params structure.
 *
 * @param params Pointer to the shared simulation parameters (t_params *).
 * @return long Elapsed time in milliseconds since simulation start.
 */
long	get_timestamp_ms(t_params *params)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	return ((now.tv_sec - params->start_time.tv_sec) * 1000
		+ (now.tv_usec - params->start_time.tv_usec) / 1000);
}

/**
 * @brief Returns the current time in milliseconds.
 *
 * Uses gettimeofday() to get the time and converts it to milliseconds.
 *
 * @return long Current timestamp in milliseconds.
 */
long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

/**
 * @brief Checks if the simulation has been stopped (philosopher is dead).
 *
 * Locks the stop_mutex to safely read the stop flag from the shared params.
 *
 * @param philo Pointer to the philosopher structure (t_philo *).
 * @return int Returns 1 if the simulation is stopped, 0 otherwise.
 */
int	is_dead(t_philo *philo)
{
	int	dead;

	pthread_mutex_lock(&philo->params->print_mutex);
	dead = philo->params->stop;
	pthread_mutex_unlock(&philo->params->print_mutex);
	return (dead);
}

/**
 * @brief Sleeps for a given amount of milliseconds, checking if philosopher
 *			died during sleep.
 *
 * Uses a loop with short usleep intervals to periodically check if the
 * philosopher should stop sleeping.
 *
 * @param ms Time to sleep in milliseconds.
 * @param philo Pointer to the philosopher structure (t_philo *).
 */
void	smart_sleep(long ms, t_philo *philo)
{
	long	start;

	start = get_time();
	while (!is_dead(philo) && get_time() - start < ms)
		usleep(100);
}
