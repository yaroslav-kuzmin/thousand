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
/*   Email contact: kuzmin.yaroslav@gmail.com                                */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************/
/* Дополнительные файлы                                                      */
/*****************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <stdint.h>
#include <openssl/md5.h>

#include <glib.h>

#include "pub.h"
#include "alloc.h"
#include "total.h"
#include "warning.h"
#include "log.h"
#include "ini.h"
#include "bit_flag.h"

#include "kernel.h"
#include "net_server.h"
#include "protocol.h"
#include "list_user.h"
#include "access.h"
#include "list_acting.h"

/*****************************************************************************/
/* Глобальные переменые                                                      */
/*****************************************************************************/

const char * programm_name;

const char * const short_options = "hVl";
const struct option long_options[] = 
{
	{"help",    0, NULL, 'h'},
	{"version", 0, NULL, 'V'},
	{"log",     0, NULL, 'l'},
	{NULL,      0, NULL,  0 }
};

/*****************************************************************************/
/* Вспомогательные функция                                                   */
/*****************************************************************************/

void print_help(FILE * stream)
{
	fprintf(stream,"использоание программы : %s [ОПЦИИ] \n",programm_name);
	fprintf(stream,"    -h  --help      Выводит помощь \n"
	               "    -V  --version   Версия программы\n"
	               "    -l  --log       Включить запись действий в программе\n");
}

#define VERSION           "0.1"
#define DATA_COM          __DATE__" : "__TIME__
#define AUTOR             "Kuzmin Yaroslav"
#define EMAIL             "kuzmin.yaroslav@gmail.com" 

void print_version(FILE * stream)
{
	fprintf(stream,"\n  Version  %s : Data \'%s\' : Autor \'%s\' : Email \'%s\'\n\n",VERSION,DATA_COM,AUTOR,EMAIL);
}

void close_server(int signal_num)
{
	deinit_list_acting();
	deinit_access_user();
	close_soket();
	deinit_list_user();
	deinit_bit_fields();
	close_config();
	close_log_system();
	close_warning_system();
	deinit_str_alloc();
	exit(0); 
}

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

	act.sa_handler = handler_timer;
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
/* Основная функция                                                          */
/*****************************************************************************/
int main(int argc,char * argv[])
{
  	int rc = 0;
	int next_option = 0;
	programm_name = argv[0];
	for(;next_option != -1;){
 		next_option = getopt_long(argc,argv,short_options,long_options,NULL);
		switch(next_option){
			case 'h': 
				print_help(stdout);
				exit(SUCCESS);
				break;
			case 'V':
				print_version(stdout);
				exit(SUCCESS);
				break;
			case 'l':
				set_registration_operation(YES);
				break;
			case '?':
				print_help(stdout);
				exit(SUCCESS);
				break; 
			case -1:
				break;
			default:
				exit(SUCCESS);
		}
	}

/*************************************/
/* Инициализация сервера             */
/*************************************/
	rc = set_signals();
	if(rc == FAILURE)
		exit(0);
	init_str_alloc();/**/ 
	total_check();
#if 0	
	rc = init_warning_system(SERVER_FLAG);
	if(rc == FAILURE){
		fprintf(stderr,"Несмог инициализировать систему предупреждений !!");
	}
#endif	
	rc = init_log_system(SERVER_FLAG);
	if(rc == FAILURE){
		global_warning("Несмог ининциализировать систему логирования!");
	}
	rc = read_config();
	if(rc == FAILURE){
		global_warning("Несмог инициализировать конфигурацию!");
		goto exit_server;
	}
	rc = init_bit_fields();
	rc = init_list_user();
	if(rc == FAILURE){
		global_warning("Несмог инициализировать Пользователей!");
		goto exit_server;	
	}
	rc = init_socket();
	if(rc == FAILURE){
		global_warning("Несмог инициализировать локальный сокет!");
		goto exit_server;
	}
	rc = init_access_user();
	if(rc == FAILURE){
		global_warning("Несмог инициализировать сисетму доступа на сервер!");
		goto exit_server;
	}
	global_log("Инициализировал систеиу доступа на сервер!");
	rc = init_list_acting();
	if(rc == FAILURE){
		global_warning("Несмог инициализировать список игр!");
		goto exit_server;
	}
	rc = set_timer();
	if(rc == FAILURE){
		goto exit_server;
	}
	global_log("Инициализирован таймер!");
/*************************************/
/* основной цикл                     */	
/*************************************/
	main_loop();
/*************************************/
/* Завершение работы                 */	
/*************************************/
exit_server:
	close_server(SIGQUIT);
	return 0;	  
}

/*****************************************************************************/
