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
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>


#include "pub.h"
#include "alloc.h"
#include "total.h"
#include "warning.h"
#include "log.h"
#include "ini.h"

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
#define AUTOR             "Yaroslav Kuzmin"
#define EMAIL             "esdt@mail.ru" 

void print_version(FILE * stream)
{
	fprintf(stream,"\n  Version  %s : Data \'%s\' : Autor \'%s\' : Email \'%s\'\n\n",VERSION,DATA_COM,AUTOR,EMAIL);
}

void close_server(int signal_num);
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

static int set_signals(void)
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
/* Функции взаимодействия по сети                                            */
/*****************************************************************************/
#define BACKLOG              10 
int fd_local_socket = 0;

int init_socket(void)
{
	int rc;
	int s_fd;
	struct sockaddr_un ssa;
	char * local_socket;

	s_fd = socket(PF_UNIX, SOCK_STREAM, 0);
	if(s_fd == -1){
		global_warning("Немогу создать локальный сокет : %s", strerror(errno));
		return FAILURE;
	}
	global_log("Создали сокет %#x!",s_fd);

	ssa.sun_family = AF_LOCAL;
	local_socket = get_local_socket();
	strcpy(ssa.sun_path,local_socket);
	
	rc = bind(s_fd,&ssa,SUN_LEN(&ssa));
	if(rc == -1){
		global_warning("Немогу связать локальный сокет с локальным файлом %s : %s",ssa.sun_path,strerror(errno));
		if(errno == EADDRINUSE){
			unlink(local_socket);
			rc = bind(s_fd,&ssa,SUN_LEN(&ssa));
			if(rc != -1){
				goto bind_success;
			}
		}
		close(s_fd);
		return FAILURE;
	}
bind_success:
	global_log("Соединили сокет %#x с файлом %s!",s_fd,ssa.sun_path);
	rc = listen(s_fd,BACKLOG);
	if(rc == -1){
		global_warning("Немогу создать очередь : %s",strerror(errno));
		close(s_fd);
		return FAILURE;
	}
	global_log("Ожидание запросов!");

	rc = fcntl(s_fd,F_SETFL,O_NONBLOCK|O_ASYNC); /*Установить флаги работа в неблокируюшем и асинхроным режиме*/
	if(rc == -1){
		global_warning("Несмог установить режим доступа к сокету %#x",s_fd);
		close(s_fd);
		return FAILURE;
	}
	rc = fcntl(s_fd , F_SETSIG , SIGIO);/*Установить сигнал, который будет послан, 
													  когда станет возможен ввод или вывод*/
	if(rc == -1){
		global_warning("Несмог привизать сигнал к сокету");
		close(s_fd);
		return FAILURE;
	}
	rc = fcntl(s_fd , F_SETOWN , getpid()); /*Установить идентификатор процесса или группу процесса, 
															которые будут принимать сигналы SIGIO и SIGURG для 
															событий на файловом дескрипторе */
	if(rc == -1){
		global_warning("Несмог установить привязку сигналов к процессу %#x",getpid());
		close(s_fd);
		return FAILURE;
	}
	global_log("Настроили сокет %#x!",s_fd);

	fd_local_socket = s_fd;
	return SUCCESS;	
}

int close_soket(void)
{
	char * name = get_local_socket();
	close(fd_local_socket);
	unlink(name);

	return SUCCESS;
}

#define SIZE_BUFF  1024
int read_sockect(int fd)
{
	char 	buff[SIZE_BUFF];
	int rc;

	//for(;;){
		memset(buff,0,SIZE_BUFF);
		rc = recv(fd,buff,SIZE_BUFF,0);
		if(rc == -1){
			if(errno != EAGAIN){
				global_warning("Ошибка чтения сокета : %s",strerror(errno));
			}
		}
		DEBUG_PRINTF_S(buff);
		rc = strcmp(buff,"quit");
		if(rc == 0 ){
			return SUCCESS;
		}
		/*
		if(amount_sig_io <= 1){	
			amount_sig_io = 0;
			rc = pause();
		}
		else{
			amount_sig_io--;
		}
	}*/

	memset(buff,0,SIZE_BUFF);
	strcat(buff,"test send");
	rc = strlen(buff);
	DEBUG_PRINTF_S(buff);
	send(fd,buff,rc,0);
	return FAILURE;
}

int connect_socket(void)
{
	struct sockaddr_un client_name;
	socklen_t client_name_len;
	int client_socket_fd;
	int rc;

	for(;;){
		client_socket_fd = accept(fd_local_socket,&client_name,&client_name_len);
		if(client_socket_fd == -1){
			if(errno != EAGAIN){
				global_warning("Клиент не смог соединится : %s",strerror(errno));
			}
		}
		else{
			rc = read_sockect(client_socket_fd);
			if(rc == SUCCESS){
				break;
			}
		}
			
		if(amount_sig_io <= 1){	
			amount_sig_io = 0;
			rc = pause();
		}
		else{
			amount_sig_io--;
		}
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

	rc = set_signals();
	if(rc == FAILURE)
		exit(0);
	init_str_alloc(); 
	total_check();
/*************************************/
	rc = init_warning_system(SERVER_FLAG);
	if(rc == FAILURE){
		fprintf(stderr,"Несмог инициализировать систему предупреждений !!");
	}
	rc = init_log_system(SERVER_FLAG);
	if(rc == FAILURE){
		global_warning("Несмог ининциализировать систему логирования!");
	}
	rc = read_config();
	if(rc == FAILURE){
		global_warning("Несмог инициализировать конфигурацию!");
		goto exit_server;
	}
/*************************************/
	rc = init_socket();
	if(rc == FAILURE){
		global_warning("Несмог инициализировать серверное соединение!");
		goto exit_server;
	}
	global_log("Инициализировали локальный сокет!");

	connect_socket();

/*************************************/
exit_server:
	close_server(SIGQUIT);
	return 0;	  
}

void close_server(int signal_num)
{
	close_soket();
	close_config();
	close_log_system();
	close_warning_system();
	exit(0);
}

/*****************************************************************************/
