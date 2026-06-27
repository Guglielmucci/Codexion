/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fedegugl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/26 09:55:39 by fedegugl          #+#    #+#             */
/*   Updated: 2026/06/26 09:55:41 by fedegugl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "monitor.h"

void	signal_stop(t_sim *sim)
{
	int	j;

	j = 0;
	while (j < sim->num_coders)
	{
		pthread_mutex_lock(&sim->dongles[j].mutex);
		sim->dongles[j].stop = 1;
		pthread_cond_broadcast(&sim->dongles[j].cond);
		pthread_mutex_unlock(&sim->dongles[j].mutex);
		j++;
	}
}

int	all_coders_completed(t_sim *sim)
{
	int	i;
	int	completed;

	i = 0;
	while (i < sim->num_coders)
	{
		pthread_mutex_lock(&sim->coders[i].mutex);
		completed = (sim->coders[i].compiles_done >= sim->compiles_required);
		pthread_mutex_unlock(&sim->coders[i].mutex);
		if (!completed)
			return (0);
		i++;
	}
	return (1);
}
