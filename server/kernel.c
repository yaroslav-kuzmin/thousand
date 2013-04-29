/*****************************************************************************/
/*                                                                           */
/* Thousand - card game                                                      */
/*  Copyright (C) <2013> <Kuzmin Yaroslav>                                   */
/*                                                                           */
/*  This program is free software: you can redistribute it and/or modify     */
/*  it under the terms of the GNU General Public License as published by     */
/*  the Free Software Foundation, either version 3 of the License, or        */
/*  (at your option) any later version.                                      */
/*                                                                           */
/*  This program is distributed in the hope that it will be useful,          */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of           */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            */
/*  GNU General Public License for more details.                             */
/*                                                                           */
/*  You should have received a copy of the GNU General Public License        */
/*  along with this program.  If not, see <http://www.gnu.org/licenses/>.    */
/*                                                                           */
/*   Email contact: esdt@mail.ru                                             */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************/
/* Дополнительные файлы                                                      */
/*****************************************************************************/
#include <string.h>
#include <stdio.h>
#include <signal.h>

#include "pub.h"
#include "server.h"
/*****************************************************************************/
/* Глобальные переменые                                                      */
/*****************************************************************************/

/*****************************************************************************/
/* Основная функция                                                          */
/*****************************************************************************/
/* TODO
volatile sig_atomic_t amount_alrm = 0;
void handler_timer(int signal_num)
{
	amount_alrm++;
}
*/
volatile sig_atomic_t amount_sig_io = 1;
void sigaction_io(int num,siginfo_t * sig,void * test)
{
	amount_sig_io++;
}

/*****************************************************************************/
int set_signals(void)
{
	struct sigaction act;
	sigset_t set;

	/*signal action handler setup*/
	memset(&act, 0x0, sizeof(act));
	if(sigfillset(&set) < 0){
		perror("sigfillset failed");
		return FAILURE;
	}	

	act.sa_handler = SIG_IGN;
	if(sigaction(SIGHUP, &act, NULL) < 0){
		perror("sigaction failed SIGHUP");
		return FAILURE;
	}
	if(sigaction(SIGPIPE, &act, NULL) < 0){
		perror("sigaction failed SIGPIPE");
		return FAILURE;
	}
	if(sigaction(SIGCHLD, &act, NULL) < 0){
		perror("sigaction failed SIGCHLD");
		return FAILURE;
	}

	/*act.sa_handler = handler_timer; TODO*/
	if(sigaction(SIGALRM, &act, NULL) < 0){
		perror("sigaction failed SIGALRM");
		return FAILURE;
	}

	act.sa_sigaction = sigaction_io;
	act.sa_flags = SA_SIGINFO;
	if(sigaction(SIGIO, &act, NULL) < 0){
		perror("sigaction failed SIGIO");
		return FAILURE;
	}

	act.sa_handler = close_server;
	if(sigaction(SIGQUIT, &act, NULL) < 0){
		perror("sigaction failed SIGQUIT");
		return FAILURE;
	}
	if(sigaction(SIGINT, &act , NULL) < 0){
		perror("sigaction failed SIGINT");
		return FAILURE;
	}	
	if(sigaction(SIGTERM, &act, NULL) < 0){
		perror("sigaction failed SIGTERM");
		return FAILURE;
	}

	act.sa_handler = SIG_DFL;
	if(sigaction(SIGBUS, &act, NULL) < 0){
		perror("sigaction failed SIGBUS");
		return FAILURE;
	}
	if(sigaction(SIGFPE, &act, NULL) < 0){
		perror("sigaction failed SIGFPE");
		return FAILURE;
	}
	if(sigaction(SIGILL, &act, NULL) < 0){
		perror("sigaction failed SIGILL");
		return FAILURE;
	}
	if(sigaction(SIGSEGV, &act, NULL) < 0){
		perror("sigaction failed SIGSEGV");
		return FAILURE;
	}
	if(sigaction(SIGXCPU, &act, NULL) < 0){
		perror("sigaction failed SIGCPU");
		return FAILURE;
	}
	if(sigaction(SIGXFSZ, &act, NULL) < 0){
		perror("sigaction failed SIGFSZ");
		return FAILURE;
	}
	if(sigaction(SIGPWR, &act, NULL) < 0){
		perror("sigaction failed SIGPWR");
		return FAILURE;
	}
	if(sigaction(SIGSYS, &act, NULL) < 0){
		perror("sigaction failed SIGSYS");
		return FAILURE;
	}

	if(sigemptyset(&set) < 0){
		perror("sigemptyset failed");
		return FAILURE;
	}
	return SUCCESS;
}

/*****************************************************************************/
