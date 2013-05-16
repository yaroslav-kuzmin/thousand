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
#include <errno.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/un.h>

#include "pub.h"
#include "alloc.h"
#include "total.h"
#include "warning.h"
#include "log.h"
#include "ini.h"

/*****************************************************************************/
/* глобальные переменые                                                      */
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
/*****************************************************************************/
/* Функции взаимодействия по сети                                            */
/*****************************************************************************/
#define SIZE_BUFF    1024
static int fd_local_socket;
int init_socket(void)
{
	struct sockaddr_un ssa;
	char * local_socket;
	int rc;

	fd_local_socket = socket(PF_UNIX, SOCK_STREAM, 0);
	if(fd_local_socket == -1){
		global_warning("Немогу создать локальный сокет : %s", strerror(errno));
		return FAILURE;
	}
	global_log("Создали сокет %#x",fd_local_socket);
	ssa.sun_family = AF_LOCAL;
	local_socket = get_local_socket();
	strcpy(ssa.sun_path,local_socket);
	rc = connect(fd_local_socket,&ssa,SUN_LEN(&ssa));
	if(rc == -1){
		global_warning("Несмог соединится с локальным сокетом : %s",ssa.sun_path);
		close(fd_local_socket);
		return FAILURE;
	}
	global_log("Соединились с сервером!");

	return SUCCESS;
}

int write_socket(void)
{
	char buff[SIZE_BUFF];
	int size;

	memset(buff,0,SIZE_BUFF);
	strcat(buff,"quit");
	size = strlen(buff);
	send(fd_local_socket, buff , size , 0);
	global_log("Отправил на сервер %s",buff);
	memset(buff,0,SIZE_BUFF);
	size = recv(fd_local_socket,buff,SIZE_BUFF,0);
	
	DEBUG_PRINTF_S(buff);

	return SUCCESS;
}

int close_socket(void)
{
	close(fd_local_socket);
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

	init_str_alloc(); 
	total_check();
/*************************************/
	rc = init_warning_system(CLIENT_FLAG);
	if(rc == FAILURE){
		fprintf(stderr,"Несмог инициализировать систему предупреждений !!");
	}
	rc = init_log_system(CLIENT_FLAG);
	if(rc == FAILURE){
		global_warning("Несмог ининциализировать систему логирования!");
	}
	rc = read_config();
	if(rc == FAILURE){
		global_warning("Несмог инициализировать конфигурацию!");
		goto exit_client;
	}
/*************************************/
	rc = init_socket();
	if(rc == FAILURE){
		global_warning("Несмог инициализировать соединение с сервером!");
		goto exit_client;
	}
	global_log("Инициализирован связь с сервером!");

	write_socket();

/*************************************/
exit_client:
	close_socket();
	close_config();
	close_log_system();
	close_warning_system();
	return 0;	  
}


/*****************************************************************************/
