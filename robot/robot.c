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
#include <stdint.h>
#include <unistd.h>
#include <getopt.h>
#include <openssl/md5.h>
#include <string.h>

#include <glib.h>

#include "pub.h"
#include "alloc.h"
#include "total.h"
#include "warning.h"
#include "log.h"
#include "ini.h"
#include "protocol.h"
#include "net_client.h"

/*****************************************************************************/
/* глобальные переменые                                                      */
/*****************************************************************************/

const char * programm_name;
char * robot = NULL;
uint8_t number_robot = 0;
uint8_t str_passwd[MD5_DIGEST_LENGTH] = {0};
uint8_t * passwd = str_passwd;
uint16_t acting = 0;
char * str_acting = NULL;
const char * const short_options = "u:a:hVl";
const struct option long_options[] =
{
	{"user",    1, NULL, 'u'},
	{"acting",  1, NULL, 'a'},
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
	fprintf(stream,"    -u  --user      Имя игрока\n"
	               "    -a  --acting    Номер игры в формате 0xXXXX (X == [0-9A-F] )\n"
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
/*************************************/
#define SIZE_STR_ACTING  4
static char symbol_hex [] = "0123456789ABCDEF";
static int symbol2hex(int c)
{
	int h;
	/*only ASCII code*/
	if((c >= '0') && (c <= '9')) {
		h = c - '0';
	}
	else{
		if( (c >= 'A') && (c <= 'F')){
			h = c - 55;
		}
		else{
			h = -1;
		}
	}
	return h;
}
static uint16_t check_acting(void)
{
	int i;
	char *str = str_acting;
	char * cstr = NULL;
	uint16_t rc = 0;

	/*проверка на 0x*/
	if(*str != '0' ){
		return 0;
	}
	str++;
	if(*str != 'x'){
		return 0;
	}
	str++;
	for(i = 0;;i++){
		int c = *str;
		cstr = strchr(symbol_hex,c);
		if((cstr == NULL) || (*cstr == 0)){
			return 0;
		}
		rc += symbol2hex(c);
		if(i == (SIZE_STR_ACTING - 1)){
			break;
		}
		rc <<= 4;
		str++;
	}

	return rc;
}
/*************************************/
#define WAITING_ANSWER        5
#define WAITING_SECONDS       1
int access_server(void)
{
	int i;
	int rc;
	uint16_t acting_check;

g_message("cmd login");
	rc = c_cmd_login(robot);
	if(rc == FAILURE){
		return rc;
	}
g_message("cmd passwd");
	rc = c_cmd_passwd(passwd);
	if(rc == FAILURE){
		return rc;
	}
g_message("wait answer");
	for(i = 0;i < WAITING_ANSWER;i++){
		rc = c_answer_access_server();
		if(rc == NOT_DATA_OBTAIN){
			sleep(WAITING_SECONDS);
			continue;
		}
		switch(rc){
			case INCORRECT_LOGIN:
				global_log("Не совпадает имя робота : %s",robot);
				rc = FAILURE;
				break;
			case INCORRECT_PASSWORD:
				global_log("Некорректный пароль");
				rc = FAILURE;
				break;
			case INCORRECT_CMD:
				global_log("Некорректный ответ сервера");
				rc = FAILURE;
				break;
			case SUCCESS:
				break;
			default:
				rc = FAILURE;
				break;
		}
		break;
	}
g_message("answer");
	if(rc == SUCCESS){
		rc = c_cmd_join_acting(acting);
		if(rc == SUCCESS){
			for(i= 0;i <WAITING_ANSWER;i++){
				rc = c_answer_join_acting(&acting_check,&number_robot);
				if(rc == NOT_DATA_OBTAIN){
					sleep(WAITING_SECONDS);
					rc = FAILURE;
					continue;
				}
				break;
			}
			if(rc == SUCCESS){
				if(acting_check == acting){
					global_log("Присоединился к игре %#x, (%d)",acting,number_robot);
					rc = SUCCESS;
				}
				else{
					global_log("Сервер не присоединил к игре %#x : ответ %#x",acting,acting_check);
					rc = FAILURE;
 				}
			}
			else{
				global_log("Сервер не присоединил к игре %#x : %d",acting,rc);
				rc = FAILURE;
 			}
		}
		else{
			global_log("Несмог отправить команду присоединится к игре %#x",acting);
			rc = FAILURE;
 		}
	}
g_message("access server : SUCCESS");
 	return rc;
}

static all_message_u message;

static int check_message(void)
{
	int rc = SUCCESS;

	message_cmd_s * cmd = (message_cmd_s*)&message;
	switch(cmd->type){
		case CMD_CHECK_CONNECT:
			global_log("Проверка соединения!");
			break;
		case CMD_NUMBER_ROUND:
			global_log("Номер раунда : %d",cmd->msg);
			break;
		case CMD_POINT:
			global_log("Очки игроков!");
			break;
		case CMD_GAME_OVER:
			global_log("Пришла команда \'игра окончена\'!");
			rc = FAILURE;
			break;
		default:
			global_log("Пришла не известная команда : %#x",cmd->type);
			rc = FAILURE;
	 		break;
	}
	return rc;
}

int main_loop(void)
{
	int rc;

	g_message("main loop");
	global_log("Зашел в основной цикл !");

	for(;;){
		rc = c_answer_message(&message);
		if(rc != NOT_DATA_OBTAIN){
			rc = check_message();
			if(rc == FAILURE){
				global_log("Закончилась игра!");
				break;
			}
		}
		global_log("Ожидание сообщения!");
		sleep(WAITING_SECONDS);
	}

	return SUCCESS;
}

/*************************************/
void close_robot(int signal_num)
{
	close_socket();
	close_config();
	close_log_system();
	close_warning_system();
	exit(EXIT_SUCCESS);
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

	act.sa_handler = close_robot;
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
			case 'u':
				robot = optarg;
				break;
			case 'a':
				str_acting = optarg;
				acting = check_acting();
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

	if((str_acting == NULL) || (robot == NULL) || ( acting == 0)){
		print_help(stdout);
		return SUCCESS;
	}

	rc = set_signals();
	if(rc == FAILURE)
		exit(0);

	init_str_alloc();
	total_check();
/*************************************/
#if 0
	отключил
	rc = init_warning_system(ROBOT_FLAG);
	if(rc == FAILURE){
		fprintf(stderr,"Несмог инициализировать систему предупреждений !!");
	}
#endif
	rc = init_log_system(ROBOT_FLAG);
	if(rc == FAILURE){
		global_warning("Несмог ининциализировать систему логирования!");
	}
	rc = read_config();
	if(rc == FAILURE){
		global_log("Несмог инициализировать конфигурацию!");
		goto exit_robot;
	}
	rc = init_socket();
	if(rc == FAILURE){
		global_log("Несмог инициализировать соединение с сервером!");
		goto exit_robot;
	}
	global_log("Инициализирован связь с сервером!");

/*************************************/

	g_message(" robot  :> %s",robot);
	g_message(" acting :> %#04x",acting);

	global_log("Имя робота :> %s",robot);
	global_log("Номер игры :> %#04x",acting);
	/*sleep(4); если нужно отлаживать сигналы поставить паузу*/
	rc = access_server();
	if(rc == FAILURE){
		global_log("Нет доступа на сервер !");
		goto exit_robot;
	}

	main_loop();

/*************************************/
exit_robot:
	close_robot(SIGQUIT);
	return 0;
}
/*****************************************************************************/
