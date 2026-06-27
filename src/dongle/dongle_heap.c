/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_heap.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fedegugl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/26 09:50:49 by fedegugl          #+#    #+#             */
/*   Updated: 2026/06/26 09:50:51 by fedegugl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dongle.h"

static void	heapify_up(t_dongle *d, int idx)
{
	int	parent;

	while (idx > 0)
	{
		parent = (idx - 1) / 2;
		if (d->cmp(d->heap[idx], d->heap[parent]))
		{
			swap(&d->heap[idx], &d->heap[parent]);
			idx = parent;
		}
		else
			break ;
	}
}

static void	heapify_down(t_dongle *d, int idx)
{
	int	left;
	int	right;
	int	smallest;

	while (1)
	{
		left = 2 * idx + 1;
		right = 2 * idx + 2;
		smallest = idx;
		if (left < d->heap_size && d->cmp(d->heap[left], d->heap[smallest]))
			smallest = left;
		if (right < d->heap_size && d->cmp(d->heap[right], d->heap[smallest]))
			smallest = right;
		if (smallest == idx)
			break ;
		swap(&d->heap[idx], &d->heap[smallest]);
		idx = smallest;
	}
}

int	heap_push(t_dongle *d, t_request *req)
{
	int			new_cap;
	t_request	**new_heap;
	int			i;

	if (d->heap_size == d->heap_capacity)
	{
		new_cap = d->heap_capacity * 2;
		new_heap = malloc(sizeof(t_request *) * new_cap);
		if (!new_heap)
			return (-1);
		i = -1;
		while (++i < d->heap_size)
			new_heap[i] = d->heap[i];
		free(d->heap);
		d->heap = new_heap;
		d->heap_capacity = new_cap;
	}
	d->heap[d->heap_size] = req;
	d->heap_size++;
	heapify_up(d, d->heap_size - 1);
	return (0);
}

t_request	*heap_pop(t_dongle *d)
{
	t_request	*top;

	if (d->heap_size == 0)
		return (NULL);
	top = d->heap[0];
	d->heap_size--;
	if (d->heap_size > 0)
	{
		d->heap[0] = d->heap[d->heap_size];
		heapify_down(d, 0);
	}
	return (top);
}

void	heap_remove(t_dongle *d, t_request *req)
{
	int	i;

	i = 0;
	while (i < d->heap_size)
	{
		if (d->heap[i] == req)
		{
			d->heap[i] = d->heap[d->heap_size - 1];
			d->heap_size--;
			if (i < d->heap_size)
			{
				heapify_down(d, i);
				heapify_up(d, i);
			}
			return ;
		}
		i++;
	}
}
