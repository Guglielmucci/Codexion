/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_log.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fedegugl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/26 09:56:14 by fedegugl          #+#    #+#             */
/*   Updated: 2026/06/26 09:56:17 by fedegugl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	print_log(t_sim *sim, int coder_id, const char *msg)
{
	long long	now_rel;

	pthread_mutex_lock(&sim->print_mutex);
	now_rel = get_time_ms() - sim->sim_start_abs;
	printf("%lld %d %s\n", now_rel, coder_id, msg);
	pthread_mutex_unlock(&sim->print_mutex);
}
