/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpushkar <vpushkar@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 12:10:50 by vpushkar          #+#    #+#             */
/*   Updated: 2025/09/23 14:22:12 by vpushkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

/**
 * @brief Check if a philosopher has died.
 *
 * This function compares the current time with the philosopher's
 * last meal time and determines if the philosopher has exceeded
 * the allowed time_to_die. If so, it sets the stop flag, prints
 * the death message, and returns 1.
 *
 * @param philo Pointer to the philosopher structure.
 * @param params Pointer to the simulation parameters.
 * @param current_time Current timestamp in milliseconds.
 *
 * @return 1 if the philosopher has died, 0 otherwise.
 */
static int	check_philo_death(t_philo *philo,
		t_params *params, long current_time)
{
	if ((philo->last_meal_time == 0 && current_time > params->time_to_die)
		|| (philo->last_meal_time > 0
			&& current_time - philo->last_meal_time > params->time_to_die))
	{
		pthread_mutex_lock(&params->print_mutex);
		params->stop = 1;
		printf("%ld %d died\n", get_timestamp_ms(params),
			philo->philo_id + 1);
		pthread_mutex_unlock(&params->print_mutex);
		return (1);
	}
	return (0);
}

/**
 * @brief Initialize the all_ate_enough flag based on must_eat_count.
 *
 * Sets the all_ate_enough flag to 1 if must_eat_count is positive,
 * otherwise sets it to 0.
 *
 * @param params Pointer to t_params containing simulation parameters.
 * @param all_ate_enough Pointer to an integer flag to initialize.
 */
static void	init_all_ate_flag(t_params *params, int *all_ate_enough)
{
	if (params->must_eat_count > 0)
		*all_ate_enough = 1;
	else
		*all_ate_enough = 0;
}

/**
 * @brief Check if all philosophers have eaten enough times.
 *
 * This function verifies whether each philosopher has reached the
 * required must_eat_count. If all have eaten enough, it sets the
 * stop flag, prints a message, and returns 1.
 *
 * @param philos Array of philosopher structures.
 * @param params Pointer to the simulation parameters.
 * @param all_ate_enough Pointer to a flag updated during the check.
 *
 * @return 1 if all philosophers ate enough, 0 otherwise.
 */
static int	check_all_ate(t_philo *philos,
		t_params *params, int *all_ate_enough)
{
	int	i;

	if (params->must_eat_count <= 0)
		return (0);
	i = 0;
	while (i < params->n)
	{
		pthread_mutex_lock(&philos[i].meal_mutex);
		if (philos[i].eat_count < params->must_eat_count)
			*all_ate_enough = 0;
		pthread_mutex_unlock(&philos[i].meal_mutex);
		i++;
	}
	if (*all_ate_enough)
	{
		pthread_mutex_lock(&params->print_mutex);
		params->stop = 1;
		printf("All philosophers have eaten at least %ld times\n",
			params->must_eat_count);
		pthread_mutex_unlock(&params->print_mutex);
		return (1);
	}
	return (0);
}

/**
 * @brief Monitor philosophers to detect death or if all have eaten enough.
 *
 * This function runs in a separate thread and continuously checks each
 * philosopher's last meal time to determine if they have died. It also
 * checks if all philosophers have eaten the required number of times and
 * stops the simulation accordingly.
 *
 * @param arg Pointer to the array of t_philo structures.
 * @return NULL always, used as thread routine.
 */
void	*monitor_routine(void *arg)
{
	t_philo		*philos;
	t_params	*params;
	int			i;
	long		current_time;
	int			all_ate_enough;

	philos = (t_philo *)arg;
	params = philos[0].params;
	while (1)
	{
		init_all_ate_flag(params, &all_ate_enough);
		i = 0;
		while (i < params->n)
		{
			pthread_mutex_lock(&philos[i].meal_mutex);
			current_time = get_timestamp_ms(params);
			if (check_philo_death(&philos[i], params, current_time))
				return (pthread_mutex_unlock(&philos[i].meal_mutex), NULL);
			pthread_mutex_unlock(&philos[i].meal_mutex);
			i++;
		}
		if (check_all_ate(philos, params, &all_ate_enough))
			return (NULL);
		usleep(10);
	}
}
