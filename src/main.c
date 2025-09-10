/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpushkar <vpushkar@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 13:58:07 by vpushkar          #+#    #+#             */
/*   Updated: 2025/09/10 14:24:12 by vpushkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

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
	if (start_simulation(philos, &params))
		return (1);
	free(philos);
	free(params.forks);
	return (0);
}
