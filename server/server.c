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
#include <string.h>
#include <errno.h>
#include <signal.h>

#include "pub.h"
#include "alloc.h"
#include "total.h"
#include "warning.h"
#include "log.h"
#include "ini.h"
#include "kernel.h"
#include "net_server.h"
#include "list_user.h"

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

void close_server(int signal_num)
{
	/*close_soket();*/
	deinit_list_user();
	close_config();
	close_log_system();
	close_warning_system();
	deinit_str_alloc();
	exit(0);
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
	rc = init_list_user();
	if(rc == FAILURE){
		global_warning("Несмог инициализировать Пользователей!")
		goto exit_server;	
	}
/*************************************/
/*	
	rc = init_socket();
	if(rc == FAILURE){
		global_warning("Несмог инициализировать серверное соединение!");
		goto exit_server;
	}
	global_log("Инициализировали локальный сокет!");

	connect_socket();
*/
/*************************************/
exit_server:
	close_server(SUCCESS);
	return 0;	  
}

/*****************************************************************************/
