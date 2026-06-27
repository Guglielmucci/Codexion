/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fedegugl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/26 09:51:23 by fedegugl          #+#    #+#             */
/*   Updated: 2026/06/26 09:51:25 by fedegugl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dongle.h"

void	swap(t_request **a, t_request **b)
{
	t_request	*tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

t_request	*enqueue_request(t_dongle *d, int coder_id,
	long long deadline)
{
	t_request	*req;

	req = malloc(sizeof(t_request));
	if (!req)
		return (NULL);
	req->coder_id = coder_id;
	req->request_time = get_time_ms();
	req->deadline = deadline;
	if (heap_push(d, req) == -1)
	{
		free(req);
		return (NULL);
	}
	return (req);
}

int	handle_timeout_or_stop(t_dongle *d, t_request *req,
	long long now, long long deadline)
{
	if (now >= deadline || d->stop)
	{
		heap_remove(d, req);
		free(req);
		return (-1);
	}
	return (0);
}
