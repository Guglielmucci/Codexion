/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fedegugl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/26 09:57:44 by fedegugl          #+#    #+#             */
/*   Updated: 2026/06/26 09:57:46 by fedegugl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DONGLE_H
# define DONGLE_H

# include "../includes/codexion.h"

t_request	*heap_pop(t_dongle *d);
t_request	*enqueue_request(t_dongle *d, int coder_id,
				long long deadline);
int			heap_cmp_fifo(t_request *a, t_request *b);
int			heap_cmp_edf(t_request *a, t_request *b);
int			heap_push(t_dongle *d, t_request *req);
void		heap_remove(t_dongle *d, t_request *req);
int			init_dongles(t_sim *sim);
int			try_take_dongle(t_sim *sim, int coder_id, int dongle_id,
				long long timeout_abs);
void		swap(t_request **a, t_request **b);
int			handle_timeout_or_stop(t_dongle *d, t_request *req,
				long long now, long long deadline);

#endif
