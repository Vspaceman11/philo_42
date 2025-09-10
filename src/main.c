/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpushkar <vpushkar@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 13:58:07 by vpushkar          #+#    #+#             */
/*   Updated: 2025/09/10 15:55:29 by vpushkar         ###   ########.fr       */
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
	if (pthread_create(&monitor, NULL, &monitor_routine, philos) != 0)
		return (print_error("Failed to create monitor thread"));

	i = 0;
	while (i < params.n)
	{
		if (pthread_create(&philos[i].thread, NULL, &philo_routine, &philos[i]) != 0)
			return (print_error("Failed to create philosopher thread"));
		usleep(100);
		i++;
	}
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


