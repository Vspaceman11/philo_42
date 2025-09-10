/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_simulation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpushkar <vpushkar@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 13:51:41 by vpushkar          #+#    #+#             */
/*   Updated: 2025/09/10 14:25:54 by vpushkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

/**
 * @brief Thread routine for each philosopher.
 *
 * @param arg Pointer to t_philo struct.
 * @return NULL
 */
static void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	printf("Philosopher %d started\n", philo->philo_id);
	return (NULL);
}

/**
 * @brief Start simulation by creating and joining philosopher threads.
 *
 * @param philos Array of philosophers.
 * @param params Simulation parameters.
 * @return 0 on success, 1 on error.
 */
int	start_simulation(t_philo *philos, t_params *params)
{
	int	i;

	i = 0;
	while (i < params->n)
	{
		if (pthread_create(&philos[i].thread, NULL,
				&philo_routine, &philos[i]) != 0)
			return (print_error("Failed to create thread"));
		i++;
	}
	i = 0;
	while (i < params->n)
	{
		if (pthread_join(philos[i].thread, NULL) != 0)
			return (print_error("Failed to join thread"));
		i++;
	}
	return (0);
}

