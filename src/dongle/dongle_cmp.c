/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_cmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fedegugl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/26 09:50:35 by fedegugl          #+#    #+#             */
/*   Updated: 2026/06/26 09:50:37 by fedegugl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dongle.h"

int	heap_cmp_fifo(t_request *a, t_request *b)
{
	if (a->request_time != b->request_time)
		return (a->request_time < b->request_time);
	return (a->coder_id < b->coder_id);
}

int	heap_cmp_edf(t_request *a, t_request *b)
{
	if (a->deadline != b->deadline)
		return (a->deadline < b->deadline);
	return (a->coder_id < b->coder_id);
}
