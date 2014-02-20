/*****************************************************************************/
/*                                                                           */
/*Thousand - card game                                                       */
/* Copyright (C) <2013> <Kuzmin Yaroslav>                                    */
/*                                                                           */
/* This program is free software: you can redistribute it and/or modify      */
/* it under the terms of the GNU General Public License as published by      */
/* the Free Software Foundation, either version 3 of the License, or         */
/* (at your option) any later version.                                       */
/*                                                                           */
/* This program is distributed in the hope that it will be useful,           */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of            */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             */
/* GNU General Public License for more details.                              */
/*                                                                           */
/* You should have received a copy of the GNU General Public License         */
/* along with this program.  If not, see <http://www.gnu.org/licenses/>.     */
/*                                                                           */
/*  Email contact: kuzmin.yaroslav@gmail.com                                 */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************/
/* Модуль обработки игр                                                      */
/*****************************************************************************/

/*****************************************************************************/
/* Дополнительные файлы                                                      */
/*****************************************************************************/
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <openssl/md5.h>

#include <glib.h>

#include "pub.h"
#include "protocol.h"
#include "log.h"
#include "warning.h"
#include "bit_flag.h"
#include "cards.h"

#include "list_user.h"
#include "list_message.h"
#include "list_robot.h"

#include "decks.h"
#include "net_server.h"

/*****************************************************************************/
/* Глобальные переменые                                                      */
/*****************************************************************************/
#define AMOUNT_PLAYER      3
#define PLAYER_CREATOR     0
#define PLAYER_CENTR       PLAYER_CREATOR
#define PLAYER_LEFT        1
#define PLAYER_RIGHT       2

typedef enum _acting_flag_e acting_flag_e;

enum _acting_flag_e{
	all_join_acting,
	begin_round,
	auction_round,
	play_round,
	end_round,
	last_flag_acting
};
typedef struct _acting_s acting_s;
struct _acting_s
{
 	uint16_t number;
	uint32_t flag;
 	user_s * player[AMOUNT_PLAYER];
	uint16_t round;
	uint16_t points[AMOUNT_PLAYER];
	uint8_t bolt[AMOUNT_PLAYER];
	status_player_e status_player[AMOUNT_PLAYER];
	deck_cards_s * deck;
};

static GHashTable * all_acting = NULL;
static GRand * fortune = NULL;
/*****************************************************************************/
/* Вспомогательные функция                                                   */
/*****************************************************************************/
uint32_t acting_hash(gconstpointer key)
{
	acting_s * tk = (acting_s *)key;

	return (uint32_t)tk->number;
}
int acting_equal(gconstpointer a,gconstpointer b)
{
	uint16_t ta = ((acting_s  *)a)->number;
	uint16_t tb = ((acting_s*)b)->number;
	if(ta == tb){
		return TRUE;
	}
	return FALSE;
}
void acting_destroy(gpointer psa)
{
	acting_s * pta = (acting_s*)psa;
	uint32_t flag = pta->flag;
	deck_cards_s * d = pta->deck;
	deinit_bit_flag(flag);
	g_slice_free1(sizeof(deck_cards_s),d);
 	g_slice_free1(sizeof(acting_s),psa);
}
/************************************/
static uint16_t check_number_acting(void)
{
	uint32_t i;
	acting_s ta;
	int rc;

	for(i = UINT16_MAX;i != 0;i--){
		ta.number = i;
		rc = g_hash_table_contains(all_acting,&ta);
		if(rc == FALSE){
			return (uint16_t) i;
		}
	}

	return END_NUMBER_ACTING;
}
static int create_acting(user_s * psu)
{
	uint32_t flag = psu->flag;
	int rc;
	uint16_t number;
	acting_s * pta = NULL;

	rc = check_bit_flag(flag,robot_user,1);
	if(rc == YES){
		global_log("Робот не может создавать игру!");
		return ROBOT_CAN_NOT_CREATE_ACTING;
	}

	number = check_number_acting();
	if(number == END_NUMBER_ACTING){
		psu->acting = 0;
		global_log("Создано максимально возможное колличество игр!");
		return LIMIT_ACTING;
	}

	pta = g_slice_alloc0(sizeof(acting_s));
	pta->number = number;
	pta->player[PLAYER_CREATOR] = psu;
	pta->flag = init_bit_flags(last_flag_acting);
	pta->deck = g_slice_alloc(sizeof(deck_cards_s));
	g_hash_table_add(all_acting,pta);

	set_bit_flag(flag,acting_user,1);
	psu->acting = number;

	global_log("Создал новою игру : 0x%04x!",pta->number);
	return  SUCCESS;
}

static int rand_dealer(void)
{
	int rc = g_rand_int_range(fortune,0,AMOUNT_PLAYER);
	return rc;
}
/* передается номер текушего раздающего , идет смена */
static change_status_players(acting_s * psa,uint8_t player)
{
	if(player == PLAYER_CENTR){
		psa->status_player[PLAYER_CENTR] = free_player;
		psa->status_player[PLAYER_LEFT] = dealer_player;
		psa->status_player[PLAYER_RIGHT] = automat_player;
	}
	if(player == PLAYER_LEFT){
		psa->status_player[PLAYER_CENTR] = automat_player;
		psa->status_player[PLAYER_LEFT] = free_player;
		psa->status_player[PLAYER_RIGHT] = dealer_player;
	}
	if(player == PLAYER_RIGHT){
		psa->status_player[PLAYER_CENTR] = dealer_player;
		psa->status_player[PLAYER_LEFT] = automat_player;
		psa->status_player[PLAYER_RIGHT] = free_player;
	}

	return SUCCESS;
}

#define FIRST_ROUND    1
static int init_round(acting_s * psa)
{
	uint32_t flag = psa->flag;
	int p = 0;

	set_bit_flag(flag,all_join_acting,1);
	set_bit_flag(flag,begin_round,1);

	p = rand_dealer();
	change_status_players(psa,p);

	psa->round = FIRST_ROUND;
	psa->points[PLAYER_CENTR] = 0;
	psa->points[PLAYER_LEFT] = 0;
	psa->points[PLAYER_RIGHT] = 0;
	psa->bolt[PLAYER_CENTR] = 0;
	psa->bolt[PLAYER_LEFT] = 0;
	psa->bolt[PLAYER_RIGHT] = 0;

	return SUCCESS;
}

static int join_acting(user_s * psu,uint16_t number)
{
	int rc,i,c;
	uint32_t flag = psu->flag;
	acting_s ta;
	acting_s * pta;
	uint8_t number_player;

	ta.number = number;
	rc = g_hash_table_lookup_extended(all_acting,(gpointer)&ta,(gpointer *)&pta,(gpointer*)&pta);
	if(rc == FALSE){
		psu->acting = 0;
		global_log("Нет такой игры 0x%04x в списке",number);
		return FAILURE;
	}

	if(pta->player[PLAYER_LEFT] == NULL){
		pta->player[PLAYER_LEFT] = psu;
		number_player = PLAYER_LEFT;
	}
	else{
		if(pta->player[PLAYER_RIGHT] == NULL){
			pta->player[PLAYER_RIGHT] = psu;
			number_player = PLAYER_RIGHT;
		}
		else{
			global_log("Игра 0x%04x занята");
			psu->acting = 0;
	 		return FAILURE;
	 	}
	}

	set_bit_flag(flag,acting_user,1);
	psu->acting = pta->number;

	for(c = 0,i = 0;i < AMOUNT_PLAYER;i++){
		user_s * opsu = pta->player[i];
		if(opsu == NULL){ /*нет игрока*/
			continue;
		}
		c++;
		if( opsu == psu){/*это текущий игрок*/
			continue;
		}
		flag = opsu->flag;
		rc = check_bit_flag(flag,robot_user,1);
		if(rc == YES){
			continue;
		}
		rc = s_cmd_join_player(opsu,number_player,psu->name);
		if(rc == FAILURE){
			del_user_list(opsu->fd,NOT_ACTING_DEL);
			c--;
		}
		else{
			global_log("Отправил игроку %s сообщение о присоединени игрока %s в игру 0x%04x"
			          ,opsu->name,psu->name,pta->number);
		}
	}

	if(c == i){
		init_round(pta);
	}

	return number_player;
}
static int check_new_acting(user_s * psu)
{
	message_cmd_s * cmd;
	int rc;

	rc = read_message_list(psu,(uint8_t **)&cmd);
	if(rc < sizeof(message_cmd_s)){
		return FAILURE;
	}

	if(cmd->type == CMD_NEW_ACTING){
		rc = create_acting(psu);
		if(rc != SUCCESS){
			return FAILURE;
		}
		del_message_list(psu,sizeof(message_cmd_s));
		rc = s_answer_new_acting(psu,psu->acting,PLAYER_CREATOR);
		if(rc != FAILURE){
			run_robot(psu->acting);
			run_robot(psu->acting);
		}
	}
	else{
		if(cmd->type == CMD_JOIN_ACTING){
			if(cmd->msg != END_NUMBER_ACTING){
				rc = join_acting(psu,cmd->msg);
				del_message_list(psu,sizeof(message_cmd_s));
			}
			else{/*0 игры нет*/
				/* TODO вернуть список игр */
				global_log("Пока не возврашаем список игр");
				del_message_list(psu,sizeof(message_cmd_s));
				psu->acting = END_NUMBER_ACTING;
				del_user_list(psu->fd,NOT_ACTING_DEL);
				return SUCCESS;
			}
			if(rc != FAILURE){/*присоеденил к запрошеной игре*/
				rc = s_answer_join_acting(psu,psu->acting,rc);
			}
			else{/*несмог присоеденить к запрошеной игре*/
				rc = s_answer_join_acting(psu,END_NUMBER_ACTING,PLAYER_CENTR);
			}
		}
		else{
			global_log("Некоректная комманда от игрока %s : %d",psu->name,psu->fd);
			del_user_list(psu->fd,NOT_ACTING_DEL);
			return SUCCESS;
		}
	}

	if(rc == FAILURE){
		global_log("Несмог отправить сообщение игроку %d на создание игры : %s",psu->fd,strerror(errno));
		del_user_list(psu->fd,NOT_ACTING_DEL);
		return SUCCESS;
	}

	if(psu->acting == 0){
		global_log("Несмог присоединить игрока %s : %d к игре",psu->name,psu->fd);
	}
	else{
		global_log("Присоединил игрока %s : %d к игре 0x%04x",psu->name,psu->fd,psu->acting);
	}

 	return SUCCESS;
}
/************************************/

static int delete_acting(acting_s * psa,user_s * psu)
{
	int i;
	user_s * ptu;


	for(i = 0;i < AMOUNT_PLAYER;i++){
		ptu = psa->player[i];
		if(ptu == NULL){
			continue;
		}
		if(ptu != psu){
			s_cmd_game_over(ptu,psa->number);
		}
		del_user_list(ptu->fd,ACTING_DEl);
	}

	global_log("Удаление игры под номером 0x%04x",psa->number);

	return TRUE;
}

static int check_begin_round(acting_s * psa)
{
	int i;
	int rc;
	user_s * ptu;
	/*uint32_t flag;*/
	deck_cards_s * psd = psa->deck;

	generate_deck(psd);

	for(i = 0;i < AMOUNT_PLAYER;i++){
		ptu = psa->player[i];
		rc = s_cmd_number_round(ptu,psa->round);
		rc = s_cmd_amount_point_player(ptu,PLAYER_CENTR
		                              ,psa->points[PLAYER_CENTR],psa->bolt[PLAYER_CENTR]);
		rc = s_cmd_amount_point_player(ptu,PLAYER_LEFT
		                              ,psa->points[PLAYER_LEFT],psa->bolt[PLAYER_LEFT]);
		rc = s_cmd_amount_point_player(ptu,PLAYER_RIGHT
		                              ,psa->points[PLAYER_RIGHT],psa->bolt[PLAYER_RIGHT]);
		rc = s_cmd_statys_player(ptu,PLAYER_CENTR,psa->status_player[PLAYER_CENTR]);
		rc = s_cmd_statys_player(ptu,PLAYER_LEFT,psa->status_player[PLAYER_LEFT]);
		rc = s_cmd_statys_player(ptu,PLAYER_RIGHT,psa->status_player[PLAYER_RIGHT]);
		if(rc == FAILURE){
			del_user_list(ptu->fd,NOT_ACTING_DEL);
			return SUCCESS;
		}
	}
	return SUCCESS;
}

static int check_auction_round(acting_s * pas)
{
	return SUCCESS;
}

static int check_play_round(acting_s * psa)
{
	return SUCCESS;
}

static int check_end_round(acting_s * psa)
{
	return SUCCESS;
}

static int check_acting_server(acting_s * psa)
{
	int rc;
	uint32_t flag = psa->flag;

	rc = check_bit_flag(flag,all_join_acting,1);
	if(rc == NO){
		return FAILURE;
	}

	rc = check_bit_flag(flag,begin_round,1);
	if(rc == YES){
		rc = check_begin_round(psa);
		if(rc == SUCCESS){
			unset_bit_flag(flag,begin_round,1);
			set_bit_flag(flag,auction_round,1);
		}
	}
	check_auction_round(psa);
	check_play_round(psa);
	check_end_round(psa);

	return SUCCESS;
}

static int check_acting_user(user_s * psu)
{
	message_cmd_s *  cmd;
	int rc = SUCCESS;
	int lm;

	lm = read_message_list(psu,(uint8_t **)&cmd);
	if(lm < sizeof(message_cmd_s)){
		return FAILURE;
	}
	switch(cmd->type){
	 	case CMD_CHECK_CONNECT:
	 		global_log( "проверка соединения от игрока : %s",psu->name);
			break;
		case CMD_GAME_OVER:
	 		global_log("Команда \'завершение игры\' от игрока : %s",psu->name);
			rc = DELETE_ACTING_USER;
			break;
		default:
			global_log("Неизвестная команда %#x от игрока : %s",psu->name);
			del_user_list(psu->fd,NOT_ACTING_DEL);
			break;
	}
	del_message_list(psu,lm);
	return rc;
}

static gboolean check_acting(gpointer psa,gpointer dupsa,gpointer data)
{
	int rc;
	int i;
	acting_s * pta = (acting_s *)psa;
	user_s * ptu;
	uint32_t flag;

	rc = check_acting_server(pta);
	if(rc == DELETE_ACTING_SERVER){
		return delete_acting(pta,NULL);
	}

	for(i = 0;i < AMOUNT_PLAYER;i++){
		ptu = pta->player[i];
		if(ptu == NULL){
			continue;
		}
		flag = ptu->flag;

		rc = check_bit_flag(flag,acting_user,1);
		if(rc == NO){
			continue;
		}
		rc = check_bit_flag(flag,message_user,1);
		if(rc == NO){
			continue;
		}
		rc = check_acting_user(ptu);
		if(rc == DELETE_ACTING_USER){
			return delete_acting(pta,ptu);
		}
	}

	return FALSE;
}

/*****************************************************************************/
/* Основная функция                                                          */
/*****************************************************************************/
int init_list_acting(void)
{
	all_acting = g_hash_table_new_full(acting_hash,acting_equal,acting_destroy,NULL);
	fortune = g_rand_new();
	global_log("Создал список игр!");
	return SUCCESS;
}

int deinit_list_acting(void)
{
	g_hash_table_destroy(all_acting);
	g_rand_free(fortune);
	global_log("Удалил список игр!");
	return SUCCESS;
}

int create_actings(int * success)
{
	int rc;
	user_s * ptu;
	uint16_t flag;

	ptu = get_first_user_list();
	for(;ptu != NULL;ptu = get_next_user_list()){
		flag = ptu->flag;
		rc = check_bit_flag(flag,access_server_user,1);
		if(rc == NO){
			continue;
		}
		rc = check_bit_flag(flag,acting_user,1);
		if(rc == YES){
			continue;
		}
		rc = check_bit_flag(flag,message_user,1);
		if(rc == NO){
			continue;
		}
		rc = check_new_acting(ptu);
		if(rc == SUCCESS){
			(*success)--;
		}
	}
	return SUCCESS;
}

int current_actings(void)
{
	int rc;

	g_message("current acting : %p",all_acting);

	rc = g_hash_table_foreach_remove(all_acting,check_acting,NULL);

	global_log("Проверили текущие игры : %d",rc);

 	return SUCCESS;
}

/*TODO Сделать более интелектуальное отсоединения игроков от игры */
/*     в зависимости кто вышел и кто остался */
/*     или перезапустить робота или удалить всю игру*/
int delete_user_acting(user_s * psu)
{
	int rc;
	acting_s ta;
	acting_s * pta;
	uint32_t flag;

	if(psu->acting == 0){
		return FAILURE;
	}
	ta.number = psu->acting;

	rc = g_hash_table_lookup_extended(all_acting,(gpointer)&ta,(gpointer *)&pta,(gpointer*)&pta);
	if(rc == FALSE){
		global_log("Нет такой игры 0x%04x в списке",ta.number);
		return FAILURE;
	}

	if(psu == pta->player[PLAYER_CENTR]){
		pta->player[PLAYER_CENTR] = NULL;
	}
	if(psu == pta->player[PLAYER_LEFT]){
		pta->player[PLAYER_LEFT] = NULL;
	}
	if(psu == pta->player[PLAYER_RIGHT]){
		pta->player[PLAYER_RIGHT] = NULL;
	}

	flag = pta->flag;
	unset_bit_flag(flag,all_join_acting,1);

	global_log("Игрок %s вышел из игры 0x%04x!",psu->name,pta->number);

	return SUCCESS;
}
/*****************************************************************************/
