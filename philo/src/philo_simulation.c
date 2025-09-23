/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_simulation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpushkar <vpushkar@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 13:51:41 by vpushkar          #+#    #+#             */
/*   Updated: 2025/09/23 14:08:30 by vpushkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

/**
 * @brief Makes the philosopher pick up both forks.
 *
 * Depending on the philosopher's ID (even/odd), locks the forks in a specific
 * order to avoid deadlocks. Prints the "has taken a fork" state for each fork.
 *
 * @param philo Pointer to the philosopher structure (t_philo *).
 * @return int Returns 1 after successfully taking both forks.
 */
static int	take_forks(t_philo *philo)
{
	t_params	*params;
	int			left;
	int			right;

	params = philo->params;
	left = philo->philo_id;
	right = (philo->philo_id + 1) % params->n;
	if (philo->philo_id % 2 == 0)
	{
		pthread_mutex_lock(&params->forks[right]);
		print_state(philo, "has taken a fork");
		pthread_mutex_lock(&params->forks[left]);
		print_state(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&params->forks[left]);
		print_state(philo, "has taken a fork");
		pthread_mutex_lock(&params->forks[right]);
		print_state(philo, "has taken a fork");
	}
	return (1);
}

/**
 * @brief Makes the philosopher eat.
 *
 * The philosopher tries to take forks. If successful, updates the last meal
 * timestamp and eat count, prints the "is eating" state, and sleeps for the
 * eating duration. Releases the forks after eating.
 *
 * @param philo Pointer to the philosopher structure (t_philo *).
 * @return int Returns 1 if the philosopher successfully ate, 0 otherwise.
 */
int	philo_eat(t_philo *philo)
{
	t_params	*params;

	params = philo->params;
	if (!take_forks(philo))
		return (0);
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal_time = get_timestamp_ms(params);
	philo->eat_count++;
	pthread_mutex_unlock(&philo->meal_mutex);
	print_state(philo, "is eating");
	smart_sleep(params->time_to_eat, philo);
	pthread_mutex_unlock(&params->forks[philo->philo_id]);
	pthread_mutex_unlock(&params->forks[(philo->philo_id + 1) % params->n]);
	return (1);
}

/**
 * @brief Makes the philosopher sleep for the specified time.
 *
 * Prints the "is sleeping" state and calls smart_sleep to pause execution
 * for the philosopher's sleep duration.
 *
 * @param philo Pointer to the philosopher structure (t_philo *).
 */
void	philo_sleep(t_philo *philo)
{
	print_state(philo, "is sleeping");
	smart_sleep(philo->params->time_to_sleep, philo);
}

/**
 * @brief Makes the philosopher think.
 *
 * Prints the "is thinking" state. Adds a small delay for odd-numbered
 * philosophers when the total number of philosophers is odd.
 *
 * @param philo Pointer to the philosopher structure (t_philo *).
 */
void	philo_think(t_philo *philo)
{
	print_state(philo, "is thinking");
	if (philo->params->n % 2 != 0)
		usleep(500);
}

/**
 * @brief Routine executed by each philosopher thread.
 *
 * Determines the initial delay based on philosopher ID and total number
 * of philosophers to prevent deadlocks. Then enters a loop where the
 * philosopher eats, sleeps, and thinks until the stop flag is set or
 * eating fails.
 *
 * @param arg Pointer to the philosopher structure (t_philo *).
 * @return void* Always returns NULL.
 */
void	*philo_routine(void *arg)
{
	t_philo		*philo;

	philo = (t_philo *)arg;
	if (philo->philo_id % 2 == 0)
		usleep(philo->philo_id * 1000);
	while (1)
	{
		if (is_dead(philo))
			break ;
		if (!philo_eat(philo))
			break ;
		if (is_dead(philo))
			break ;
		philo_sleep(philo);
		if (is_dead(philo))
			break ;
		philo_think(philo);
	}
	return (NULL);
}
