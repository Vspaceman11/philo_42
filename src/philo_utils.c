/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpushkar <vpushkar@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 14:43:49 by vpushkar          #+#    #+#             */
/*   Updated: 2025/09/10 14:55:57 by vpushkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	print_state(t_philo *philo, const char *state)
{
	t_params	*params;

	params = philo->params;

	pthread_mutex_lock(&params->print_mutex);
	printf("%ld %d %s\n", get_timestamp_ms(params), philo->philo_id + 1, state);
	pthread_mutex_unlock(&params->print_mutex);
}

