/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_dongle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fedegugl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/26 09:49:31 by fedegugl          #+#    #+#             */
/*   Updated: 2026/06/26 09:49:35 by fedegugl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coder.h"
#include "dongle.h"

static void	print_msg(t_sim *sim, int idx)
{
	print_log(sim, idx, "has taken a dongle");
	print_log(sim, idx, "has taken a dongle");
}

int	take_two_dongles(t_sim *sim, int idx, long long deadline)
{
	int	first;
	int	second;

	get_dongles(sim, idx, &first, &second);
	if (try_take_dongle(sim, idx, first, deadline) == -1)
		return (-1);
	if (try_take_dongle(sim, idx, second, deadline) == -1)
	{
		release_dongle(sim, first);
		return (-1);
	}
	print_msg(sim, idx + 1);
	return (0);
}
