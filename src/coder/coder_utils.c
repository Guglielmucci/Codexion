/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fedegugl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/26 09:50:08 by fedegugl          #+#    #+#             */
/*   Updated: 2026/06/26 09:50:11 by fedegugl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coder.h"

int	is_simulation_running(t_sim *sim)
{
	int	running;

	pthread_mutex_lock(&sim->stop_mutex);
	running = !sim->stop;
	pthread_mutex_unlock(&sim->stop_mutex);
	return (running);
}

void	get_dongles(t_sim *sim, int idx, int *first, int *second)
{
	int	left;
	int	right;

	left = idx;
	right = (idx - 1 + sim->num_coders) % sim->num_coders;
	if (left < right)
	{
		*first = left;
		*second = right;
	}
	else
	{
		*first = right;
		*second = left;
	}
}

void	release_dongle(t_sim *sim, int dongle_id)
{
	t_dongle	*d;

	d = &sim->dongles[dongle_id];
	pthread_mutex_lock(&d->mutex);
	if (d->hold_count > 0)
	{
		d->hold_count--;
		if (d->hold_count == 0)
		{
			d->in_use = 0;
			d->holder_id = -1;
			d->cooldown_until = get_time_ms() + sim->dongle_cooldown;
			pthread_cond_broadcast(&d->cond);
		}
	}
	pthread_mutex_unlock(&d->mutex);
}

void	release_dongles_for_coder(t_sim *sim, int first, int second)
{
	release_dongle(sim, first);
	if (second >= 0)
		release_dongle(sim, second);
}
