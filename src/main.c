/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpushkar <vpushkar@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 13:58:07 by vpushkar          #+#    #+#             */
/*   Updated: 2025/09/17 09:37:49 by vpushkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"


int	main(int argc, char **argv)
{
	t_params	params;
	t_philo		*philos;
	pthread_t	monitor;
	int			i;

	if (init_params(&params, argc, argv))
		return (1);
	if (init_mutexes(&params))
		return (1);
	if (init_philo(&philos, &params))
		return (1);
	i = 0;
	while (i < params.n)
	{
		pthread_mutex_lock(&philos[i].meal_mutex);
		philos[i].last_meal_time = get_timestamp_ms(&params);
		pthread_mutex_unlock(&philos[i].meal_mutex);
		i++;
	}

	if (params.n == 1)
	{
		print_state(&philos[0], "has taken a fork");
		usleep(params.time_to_die * 1000);
		print_state(&philos[0], "died");
		cleanup_resources(&params, philos);
		return (0);
	}
	i = 0;
	while (i < params.n)
	{
		if (pthread_create(&philos[i].thread, NULL,
				&philo_routine, &philos[i]) != 0)
		{
			pthread_mutex_lock(&params.stop_mutex);
			params.stop = 1;
			pthread_mutex_unlock(&params.stop_mutex);
			while (i > 0)
			{
				i--;
				pthread_join(philos[i].thread, NULL);
			}
			cleanup_resources(&params, philos);
			return (print_error("Failed to create philosopher thread"));
		}
		i++;
	}
	if (pthread_create(&monitor, NULL, &monitor_routine, philos) != 0)
		return (print_error("Failed to create monitor thread"));
	pthread_join(monitor, NULL);
	i = 0;
	while (i < params.n)
	{
		pthread_join(philos[i].thread, NULL);
		i++;
	}
	cleanup_resources(&params, philos);
	return (0);
}


