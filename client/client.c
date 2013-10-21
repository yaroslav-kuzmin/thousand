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
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <openssl/md5.h>

#include <glib.h>

#include "pub.h"
#include "alloc.h"
#include "total.h"
#include "warning.h"
#include "log.h"
#include "ini.h"
#include "net_client.h"
#include "protocol.h"

#include "interface_cmd.h"
#include "interface.h"

/*****************************************************************************/
/* глобальные переменые                                                      */
/*****************************************************************************/

const char * programm_name = NULL;
char * user = NULL;
char str_user[LEN_USER_NAME] = {0};
char * passwd = NULL;
char str_passwd[LEN_USER_NAME] = {0};

uint8_t passwd_md5[MD5_DIGEST_LENGTH] = {0};
 
const char * const short_options = "u:p:hVl";
const struct option long_options[] = 
{
	{"user",    1, NULL, 'u'},
	{"passwd",  1, NULL, 'p'},
	{"help",    0, NULL, 'h'},
	{"version", 0, NULL, 'V'},
	{"log",     0, NULL, 'l'},
	{NULL,      0, NULL,  0 }
};

/*****************************************************************************/
/* Вспомогательные функция                                                   */
/*****************************************************************************/

static void print_help(FILE * stream)
{
	fprintf(stream,"использоание программы : %s [ОПЦИИ] \n",programm_name);
	fprintf(stream,"    -u  --user      имя игрока \n"
                  "    -p  --passwd    пароль \n"
                  "    -h  --help      Выводит помощь \n"
	               "    -V  --version   Версия программы\n"
	               "    -l  --log       Включить запись действий в программе\n");
}

#define VERSION           "0.1"
#define DATA_COM          __DATE__" : "__TIME__
#define AUTOR             "Kuzmin Yaroslav"
#define EMAIL             "kuzmin.yaroslav@gmail.com" 

static void print_version(FILE * stream)
{
	fprintf(stream,"\n  Version  %s : Data \'%s\' : Autor \'%s\' : Email \'%s\'\n\n",VERSION,DATA_COM,AUTOR,EMAIL);
}
/*****************************************************************************/
/* Основная функция                                                          */
/*****************************************************************************/
int close_client(void)
{
	close_interface();	
	close_socket();
	close_config();
	close_log_system();
	close_warning_system();
	exit(0);
	return SUCCESS;
}

int access_server(void)
{
	int rc;
	int type;

	if(user == NULL){
		user = str_user;
		rc = if_get_name_user((char **)&user,LEN_USER_NAME);
	}
	else{
		rc = if_set_name_user(user);
	}
	global_log("Игрок : %s",user);
	

	if(passwd == NULL){
		passwd = str_passwd;
		rc = if_get_passwd((char **)&passwd,LEN_USER_NAME);
	}
	else{
		rc = if_set_passwd(passwd);
	}

	rc = strlen((char *)passwd);
	if(rc == 0){
		global_log("Нет пароля!");
		rc = FAILURE;
		return rc;
	}
	global_log("Пароль : %s",passwd);
	MD5((uint8_t*)passwd,rc,passwd_md5);

	rc = cmd_login(user);
	if(rc == FAILURE){
		return rc;
	}
	rc = cmd_passwd(passwd_md5);
	if(rc == FAILURE){
		return rc;
	}
	
	rc = answer_access_server(&type);
	if(rc == SUCCESS){
		if_set_connect();
		rc = SUCCESS;
	}
	else{
		/*if_not_set_connetc(type);*/
		rc = FAILURE;
	}

	return rc;
}

int main_loop(void)
{
	interface_cmd_e  cmd;

	for(;;){
		cmd = if_cmd();
		if (cmd == exit_client){
			break;
		}
	}

	return SUCCESS;
}

int main(int argc,char * argv[])
{
  	int rc = 0;
	int next_option = 0;
	programm_name = argv[0];
	for(;next_option != -1;){
 		next_option = getopt_long(argc,argv,short_options,long_options,NULL);
		switch(next_option){
			case 'u':
				user = optarg;
				break;
			case 'p':
				passwd = optarg;
				break;	
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
	rc = init_socket();
	if(rc == FAILURE){
		global_warning("Несмог инициализировать соединение с сервером!");
		goto exit_client;
	}
	global_log("Инициализирован связь с сервером!");

	rc = init_interface();
	if(rc == FAILURE){
		global_warning("Несмог иництализтровать интерфейс!");
		goto exit_client;
	}
	global_log("Инициализировал интерфейс !");

/*************************************/
	rc = access_server();
	if(rc == FAILURE){
		global_log("Не корректный логин : %s и пароль : %s",user,passwd);
		goto exit_client;
	}

	main_loop();
/*************************************/
exit_client:
	close_client();
	return 0;	  
}


/*****************************************************************************/
