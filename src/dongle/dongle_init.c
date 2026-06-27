/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fedegugl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/26 09:51:01 by fedegugl          #+#    #+#             */
/*   Updated: 2026/06/26 09:51:05 by fedegugl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dongle.h"

static int	configure_dongle(t_dongle *d, int id, t_scheduler sched)
{
	d->id = id;
	pthread_mutex_init(&d->mutex, NULL);
	pthread_cond_init(&d->cond, NULL);
	d->in_use = 0;
	d->cooldown_until = 0;
	d->heap_capacity = 4;
	d->heap = malloc(sizeof(t_request *) * d->heap_capacity);
	if (!d->heap)
		return (0);
	d->heap_size = 0;
	d->hold_count = 0;
	d->holder_id = -1;
	d->stop = 0;
	d->heap_size = 0;
	if (sched == SCHEDULER_FIFO)
		d->cmp = heap_cmp_fifo;
	else if (sched == SCHEDULER_EDF)
		d->cmp = heap_cmp_edf;
	else
		d->cmp = NULL;
	return (1);
}

static int	cleanup_dongles(t_sim *sim, int i)
{
	int	j;

	j = 0;
	while (j <= i)
	{
		pthread_mutex_destroy(&sim->dongles[j].mutex);
		pthread_cond_destroy(&sim->dongles[j].cond);
		free(sim->dongles[j].heap);
		j++;
	}
	free(sim->dongles);
	sim->dongles = NULL;
	return (0);
}

int	init_dongles(t_sim *sim)
{
	int	i;

	sim->dongles = malloc(sizeof(t_dongle) * sim->num_coders);
	if (!sim->dongles)
		return (0);
	i = 0;
	while (i < sim->num_coders)
	{
		if (!configure_dongle(&sim->dongles[i], i, sim->scheduler))
			return (cleanup_dongles(sim, i));
		i++;
	}
	return (1);
}
