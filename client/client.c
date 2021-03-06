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
#include "protocol.h"
#include "net_client.h"
#include "cards.h"

#include "interface_cmd.h"
#include "interface.h"

/*****************************************************************************/
/* глобальные переменые                                                      */
/*****************************************************************************/

const char * programm_name = NULL;

#define LEN_STR_PASSWD       LEN_NAME_PLAYER

typedef struct _player_s player_s;
struct _player_s
{
	uint8_t number;
	char * name;
	char str_name[LEN_NAME_PLAYER];
	int16_t point;
	uint8_t bolt;
};

player_s player;
player_s partner_left;
player_s partner_right;

char * passwd = NULL;
char str_passwd[LEN_NAME_PLAYER] = {0};
uint16_t number_acting = 0;
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
	fprintf(stream,"    -u  --user      Имя игрока \n"
                  "    -p  --passwd    Пароль \n"
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
#define WAITING_ANSWER       5
#define WAITING_SECONDS      1
int new_acting(void)
{
	int i;
	int rc;

	rc = if_new_game();
	if(rc == FAILURE){
		return FAILURE;
	}

	rc = c_cmd_new_acting();
	if(rc == FAILURE){
		return rc;
	}
	for(i = 0; i < WAITING_ANSWER;i++){
		rc = c_answer_new_acting(&number_acting,&player.number);
		if(rc == NOT_DATA_OBTAIN){
			/*TODO отображение ожидания*/
			/*TODO определение нажатия клавиши*/
			sleep(WAITING_SECONDS);
			rc = FAILURE;
			continue;
		}
		if(number_acting == 0){
			rc = FAILURE;
	  		global_log("Сервер не создал игру %#x",number_acting);
			return rc;
		}
		else{
 			rc = SUCCESS;
	 		global_log("Сервер создал игру %#x, (%d)",number_acting,player.number);
			break;
		}
	}
	if(rc == FAILURE){
  		global_log("Сервер не ответечает!");
		return rc;
	}

	if_create_game(number_acting);

	partner_left.name = partner_left.str_name;
	for(i = 0;i< WAITING_ANSWER;i++){
		rc = c_answer_name_partner(&partner_left.number,&partner_left.name);
  		if(rc == NOT_DATA_OBTAIN){
			/*TODO отображение ожидания*/
			/*TODO определение нажатия клавиши*/
			sleep(WAITING_SECONDS);
			rc = FAILURE;
			continue;
		}
		if(rc == SUCCESS){
			global_log("К игре присоединился игрок %s : (%d)!",partner_left.name,partner_left.number);
			if_partner_left(partner_left.name);
			break;
		}
		rc = FAILURE;
		break;
	}
	if(rc == FAILURE){
		global_log("Сервер неотвичает!");
		return rc;
	}

	partner_right.name = partner_right.str_name;
	for(i = 0;i< WAITING_ANSWER;i++){
		rc = c_answer_name_partner(&partner_right.number,&partner_right.name);
  		if(rc == NOT_DATA_OBTAIN){
			/*TODO отображение ожидания*/
			/*TODO определение нажатия клавиши*/
			sleep(WAITING_SECONDS);
			rc = FAILURE;
			continue;
		}
		if(rc == SUCCESS){
			global_log("К игре присоединился игрок %s : (%d)!",partner_right.name,partner_right.number);
			if_partner_right(partner_right.name);
			break;
		}
		rc = FAILURE;
		break;
	}
	if(rc == FAILURE){
		global_log("Сервер неотвичает!");
	}

	return rc;
}

int access_server(void)
{
	int i;
	int rc;

	if(player.name == NULL){
		player.name = player.str_name;
		rc = if_get_name_player((char **)&player.name,LEN_NAME_PLAYER);
	}
	else{
		rc = if_set_name_player(player.name);
	}
	global_log("Игрок : %s",player.name);


	if(passwd == NULL){
		passwd = str_passwd;
		rc = if_get_passwd((char **)&passwd,LEN_NAME_PLAYER);
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

	rc = c_cmd_login(player.name);
	if(rc == FAILURE){
		return rc;
	}
	rc = c_cmd_passwd(passwd_md5);
	if(rc == FAILURE){
		return rc;
	}

	for(i = 0;i< WAITING_ANSWER;i++){
		rc = c_answer_access_server();
		if(rc == NOT_DATA_OBTAIN){
			sleep(WAITING_SECONDS);
			/*TODO Отрисовка ожидания*/
			/*TODO определение нажатия клавиши*/
			rc = FAILURE;
			continue;
		}
		if(rc == SUCCESS){
			if_set_connect();
			rc = SUCCESS;
			break;
		}
		if_not_set_connetc(rc);
		rc = FAILURE;
		break;
	}
	return rc;
}
/*************************************/

all_message_u message;
uint16_t max_bet = AUTOMAT_BETS;
int check_point(message_point_s * cmd)
{
	if(cmd->player == player.number){
		global_log("point :> %d : bolt :> %d",cmd->point,cmd->bolt);
		if_player_point(cmd->point);
		if_player_bolt(cmd->bolt);
 		return SUCCESS;
	}
	if(cmd->player == partner_left.number){
 		if_partner_left_point(cmd->point);
		if_partner_left_bolt(cmd->bolt);
		return SUCCESS;
	}
	if(cmd->player == partner_right.number){
		if_partner_right_point(cmd->point);
		if_partner_right_bolt(cmd->bolt);
		return SUCCESS;
	}
 	return FAILURE;
}

int check_status_player(message_status_player_s * cmd)
{
	if(cmd->number_player == player.number){
		global_log("status :> %d",cmd->status_player);
		if_player_status(cmd->status_player);
		return SUCCESS;
	}
	if(cmd->number_player == partner_left.number){
		if_partner_left_status(cmd->status_player);
		return SUCCESS;
	}
	if(cmd->number_player == partner_right.number){
		if_partner_right_status(cmd->status_player);
		return SUCCESS;
	}
	return FAILURE;
}
int check_cards(message_cards_s * cmd)
{
	int i;

	if(cmd->amount == AMOUNT_CARD_PLAYER){ /*Вначале приходит 7 карт последняя пустая*/
		if_partner_left_card((AMOUNT_CARD_PARTNER - 1)); /*одна в прикупе*/
		if_partner_right_card((AMOUNT_CARD_PARTNER - 1));
		if_table_card_left(UNKNOWN_CARD);
		if_table_card_center(UNKNOWN_CARD);
		if_table_card_right(UNKNOWN_CARD);
		for(i = 0;i < (AMOUNT_CARD_PLAYER -1);i++){
			uint8_t c = cmd->card[i];
			if_card_player(i,c,NO);
		}
	}
	return SUCCESS;
}
int check_auction(message_cmd_s * cmd)
{
	uint16_t min_bet;
	if(cmd->msg == WAIT_BETS){
		global_log("Торги : Ожидаю очереди!");
		return SUCCESS;
	}
	if(cmd->msg == PASS_BETS){
		global_log("Торги : Пасс!!");
		return SUCCESS;
	}
	/*делаем ставку */
	if(max_bet > cmd->msg){
		global_log("Ошибка : внутриния ставка %d > внешняя ставка %d",max_bet,cmd->msg);
		return FAILURE;
	}
	max_bet = cmd->msg;

	global_log("Минимальная ставка : %d",max_bet);

	if_nonblock(FALSE);
	min_bet = if_set_bet(max_bet);
	if(min_bet > max_bet){
		max_bet = min_bet;
		if_bet(max_bet);
	}
	if_nonblock(TRUE);
	global_log("Моя ставка : %d",max_bet);
	c_answer_auction(max_bet);

	return SUCCESS;
}
int check_bets(message_bets_s * cmd)
{
	if(cmd->bets == PASS_BETS){
		global_log("Игрок %d : пасс!",cmd->player);
		return SUCCESS;
	}
	if(cmd->bets == WAIT_BETS){
		global_log("Игрок %d : ожидает хода!",cmd->player);
		return SUCCESS;
	}
	if(max_bet < cmd->bets){
		max_bet = cmd->bets;
		global_log("Игрок %d : ход : %d!",cmd->player,max_bet);
		if_status_round(auction_round);
		if_bet(max_bet);
	}
	return SUCCESS;
}

int check_message(all_message_u * msg)
{
	message_cmd_s * cmd = (message_cmd_s*)msg;
	global_log("Тип сообщения : %d",cmd->type);
	switch(cmd->type){
		case CMD_NUMBER_ROUND:
			if_number_round(cmd->msg);
			if_status_round(begin_round);
			max_bet = AUTOMAT_BETS - MIN_ADD_BETS; /*для первичной прорисовки начала аукциона*/
			global_log("Номер роунда :> %d",cmd->msg);
			break;
		case CMD_POINT:
			check_point((message_point_s*)cmd);
			break;
		case MESSAGE_STATUS_PLAYER:
			check_status_player((message_status_player_s*)msg);
			break;
		case MESSAGE_CARDS:
			check_cards((message_cards_s*)msg);
			break;
		case CMD_AUCTION:
			check_auction(cmd);
			break;
		case MESSAGE_BETS:
			check_bets((message_bets_s*)msg);
			break;
		default:
			break;
	}
	return SUCCESS;
}

int main_loop(void)
{
	int rc = 0;
	interface_cmd_e  cmd;

	if_nonblock(TRUE);
	memset(&message,0,sizeof(all_message_u));
	if_table();

	for(;;){
		rc = c_answer_message(&message);
		/*global_log("Сообщение от сервера : %d",rc);*/
		if(rc != NOT_DATA_OBTAIN){
			rc = check_message(&message);
		 	memset(&message,0,sizeof(all_message_u));
		}

	 	cmd = if_cmd();
		if (cmd == exit_client){
			c_cmd_game_over(number_acting);
			break;
		}
	}

	return SUCCESS ;
}

/*************************************/
void close_client(int signal_num)
{
	close_interface();
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

	act.sa_handler = close_client;
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

	memset(&player,0,sizeof(player_s));
	memset(&partner_left,0,sizeof(player_s));
	memset(&partner_right,0,sizeof(player_s));

	for(;next_option != -1;){
 		next_option = getopt_long(argc,argv,short_options,long_options,NULL);
		switch(next_option){
			case 'u':
				player.name = optarg;
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

	rc = set_signals();
	if(rc == FAILURE)
		exit(0);

	init_str_alloc();
	total_check();
/*************************************/
#if 0
	rc = init_warning_system(CLIENT_FLAG);
	if(rc == FAILURE){
		fprintf(stderr,"Несмог инициализировать систему предупреждений !!");
	}
#endif
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
		global_log("Не корректный логин : %s или пароль : %s",player,passwd);
	 	goto exit_client;
	}

	rc = new_acting();
	if(rc == FAILURE){
		global_log("Несмог создать игру");
		goto exit_client;
	}

	main_loop();
/*************************************/
exit_client:
	close_client(SIGQUIT);
	return 0;
}


/*****************************************************************************/
