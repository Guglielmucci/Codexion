/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fedegugl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/26 09:57:31 by fedegugl          #+#    #+#             */
/*   Updated: 2026/06/26 09:57:33 by fedegugl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
/* -------------------------------------------------------------------------
   Struttura per rappresentare una richiesta in attesa di un dongle.
   ------------------------------------------------------------------------- */
typedef struct s_request
{
	int				coder_id;
	long long		request_time;
	long long		deadline;
}	t_request;

/* -------------------------------------------------------------------------
   Dongle: rappresenta una chiavetta USB posta tra due coder adiacenti.
   ------------------------------------------------------------------------- */
typedef struct s_dongle
{
	int				id;
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	int				in_use;
	long long		cooldown_until;
	t_request		**heap;
	int				heap_size;
	int				heap_capacity;
	int				hold_count;
	int				holder_id;
	int				stop;
	int				(*cmp)(t_request *, t_request *);
}	t_dongle;

/* -------------------------------------------------------------------------
   Coder: ogni thread rappresenta un programmatore.
   ------------------------------------------------------------------------- */
typedef struct s_coder
{
	int				id;
	int				compiles_done;
	long long		last_compile_start;
	int				is_compiling;
	int				started;
	pthread_mutex_t	mutex;
	pthread_t		thread;
}	t_coder;

/* -------------------------------------------------------------------------
   Enumerazione per lo scheduler, con prefisso per evitare conflitti.
   ------------------------------------------------------------------------- */
typedef enum e_scheduler
{
	SCHEDULER_FIFO,
	SCHEDULER_EDF
}	t_scheduler;

/* -------------------------------------------------------------------------
   Simulazione: contiene tutti i dati globali.
   ------------------------------------------------------------------------- */
typedef struct s_sim
{
	int				num_coders;
	int				time_to_burnout;
	int				time_to_compile;
	int				time_to_debug;
	int				time_to_refactor;
	int				compiles_required;
	int				dongle_cooldown;
	t_scheduler		scheduler;
	t_dongle		*dongles;
	t_coder			*coders;
	long long		sim_start_abs;
	int				stop;
	int				monitor_started;
	pthread_mutex_t	stop_mutex;
	pthread_mutex_t	print_mutex;
	pthread_t		monitor;
}	t_sim;

/* -------------------------------------------------------------------------
   Struttura per passare argomenti al thread coder.
   ------------------------------------------------------------------------- */
typedef struct s_coder_arg
{
	t_sim	*sim;
	int		id;
}	t_coder_arg;

/* -------------------------------------------------------------------------
   Utils
   ------------------------------------------------------------------------- */

long long	get_time_ms(void);
void		print_log(t_sim *sim, int coder_id, const char *msg);
void		ms_sleep(int ms);

#endif
