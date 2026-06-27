/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_take.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fedegugl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/26 09:51:13 by fedegugl          #+#    #+#             */
/*   Updated: 2026/06/26 09:51:15 by fedegugl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dongle.h"

static void	wait_for_dongle(t_dongle *d, long long deadline,
	t_request *req)
{
	long long		wait_until;
	struct timespec	ts;

	if (d->in_use || d->heap[0] != req)
		wait_until = deadline;
	else
	{
		wait_until = d->cooldown_until;
		if (wait_until > deadline)
			wait_until = deadline;
	}
	ts.tv_sec = wait_until / 1000;
	ts.tv_nsec = (wait_until % 1000) * 1000000;
	pthread_cond_timedwait(&d->cond, &d->mutex, &ts);
}

static int	await_dongle(t_sim *sim, t_dongle *d, t_request *req,
	long long deadline)
{
	long long	now;

	(void)sim;
	while (1)
	{
		now = get_time_ms();
		if (!(d->in_use || now < d->cooldown_until
				|| d->heap[0] != req))
			break ;
		if (handle_timeout_or_stop(d, req, now, deadline) == -1)
			return (-1);
		wait_for_dongle(d, deadline, req);
	}
	if (d->stop)
	{
		heap_remove(d, req);
		free(req);
		return (-1);
	}
	return (0);
}

static void	finish_dongle_acquisition(t_dongle *d, int coder_id,
	t_request *req)
{
	heap_pop(d);
	free(req);
	d->in_use = 1;
	d->holder_id = coder_id;
	d->hold_count = 1;
}

int	try_take_dongle(t_sim *sim, int coder_id, int dongle_id,
	long long timeout_abs)
{
	t_dongle	*d;
	t_request	*req;

	d = &sim->dongles[dongle_id];
	pthread_mutex_lock(&d->mutex);
	req = enqueue_request(d, coder_id, timeout_abs);
	if (!req)
	{
		pthread_mutex_unlock(&d->mutex);
		return (-1);
	}
	if (await_dongle(sim, d, req, timeout_abs) == -1)
	{
		pthread_mutex_unlock(&d->mutex);
		return (-1);
	}
	finish_dongle_acquisition(d, coder_id, req);
	pthread_mutex_unlock(&d->mutex);
	return (0);
}
