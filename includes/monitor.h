/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fedegugl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/26 09:58:30 by fedegugl          #+#    #+#             */
/*   Updated: 2026/06/26 09:58:33 by fedegugl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MONITOR_H
# define MONITOR_H

# include "../includes/codexion.h"

void		*monitor_routine(void *arg);
void		signal_stop(t_sim *sim);
int			all_coders_completed(t_sim *sim);

#endif
