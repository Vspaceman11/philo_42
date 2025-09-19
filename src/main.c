/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpushkar <vpushkar@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 13:58:07 by vpushkar          #+#    #+#             */
/*   Updated: 2025/09/19 13:18:14 by vpushkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

/**
 * @brief Handle the special case when there is only one philosopher.
 *
 * In the case of a single philosopher, he can only take one fork,
 * which makes it impossible to eat. This function simulates the
 * philosopher taking a fork, waiting until `time_to_die`, and then
 * printing that the philosopher has died. Afterward, it performs
 * resource cleanup.
 *
 * @param params Pointer to simulation parameters.
 * @param philos Array of philosopher structures.
 *
 * @return 1 if there was only one philosopher and the case was handled,
 *         0 otherwise.
 */
static int	handle_one_philo(t_params *params, t_philo *philos)
{
	if (params->n == 1)
	{
		print_state(&philos[0], "has taken a fork");
		usleep(params->time_to_die * 1000);
		print_state(&philos[0], "died");
		cleanup_resources(params, philos);
		return (1);
	}
	return (0);
}

/**
 * @brief Run the philosopher simulation.
 *
 * This function launches the monitor thread to supervise philosophers,
 * waits until the monitor finishes, then joins all philosopher threads.
 * After all threads have finished, it cleans up allocated resources.
 *
 * @param params Pointer to simulation parameters.
 * @param philos Array of philosopher structures.
 *
 * @return 0 on success, or a non-zero error code if the monitor thread
 *         could not be created.
 */
static int	run_simulation(t_params *params, t_philo *philos)
{
	pthread_t	monitor;
	int			i;

	if (pthread_create(&monitor, NULL, &monitor_routine, philos) != 0)
		return (print_error("Failed to create monitor thread"));
	pthread_join(monitor, NULL);
	i = 0;
	while (i < params->n)
	{
		pthread_join(philos[i].thread, NULL);
		i++;
	}
	cleanup_resources(params, philos);
	return (0);
}

int	main(int argc, char **argv)
{
	t_params	params;
	t_philo		*philos;

	if (init_params(&params, argc, argv))
		return (1);
	if (init_mutexes(&params))
		return (1);
	if (init_philo(&philos, &params))
		return (1);
	init_last_meal_times(philos, &params);
	if (handle_one_philo(&params, philos))
		return (0);
	if (start_philos_threads(&params, philos))
		return (1);
	return (run_simulation(&params, philos));
}


