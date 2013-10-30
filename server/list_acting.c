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

#include "list_user.h"
#include "list_message.h"
#include "net_server.h"
/*****************************************************************************/
/* Глобальные переменые                                                      */
/*****************************************************************************/
#define AMOUNT_PLAYER      3
#define PLAYER_CREATOR     0
#define PLAYER_CENTR       0
#define PLAYER_LEFT        1
#define PLAYER_RIGHT       2

typedef struct _acting_s acting_s;
struct _acting_s
{
	uint16_t number;
	user_s * player[AMOUNT_PLAYER];
};

static GHashTable * all_acting = NULL;
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
	uint16_t ta = ((acting_s *)a)->number;
	uint16_t tb = ((acting_s*)b)->number;
	if(ta == tb){
		return TRUE;
	}
	return FALSE;
}
void acting_destroy(gpointer psa)
{
	g_slice_free1(sizeof(acting_s),psa);
}

uint16_t check_number_acting(void)
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

	return 0;
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
	if(number == 0){
		psu->acting = 0;
		global_log("Создано максимально возможное колличество игр!");
		return LIMIT_ACTING;
	}

	pta = g_slice_alloc0(sizeof(acting_s));
	pta->number = number;
	set_bit_flag(flag,acting_user,1);
	psu->acting = number;
	pta->player[PLAYER_CREATOR] = psu;
	g_hash_table_add(all_acting,pta);

	global_log("Создал новою игру : %#04x!",pta->number);
	return  SUCCESS;
}

static int join_acting(user_s * psu,uint16_t number)
{
	int rc;
	uint32_t flag = psu->flag;
	acting_s ta;
	acting_s * pta;
	ta.number = number;
	
	rc = g_hash_table_lookup_extended(all_acting,(gpointer)&ta,(gpointer *)&pta,(gpointer*)&pta);
	if(rc == FALSE){
		psu->acting = 0;
		global_log("Нет такой игры %#04x в списке",number);
		return FAILURE;
	}
	
	if(pta->player[PLAYER_LEFT] == NULL){
		pta->player[PLAYER_LEFT] = psu;
	}
	else{
		if(pta->player[PLAYER_RIGHT] == NULL){
			pta->player[PLAYER_RIGHT] = psu;
		}
		else{
			global_log("Игра %#04x занята");
			psu->acting = 0;
			return FAILURE;
		}
	}
	set_bit_flag(flag,acting_user,1);
	psu->acting = pta->number;

	return SUCCESS;
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
		del_message_list(psu,sizeof(message_cmd_s));
	}
	else{
		if(cmd->type == CMD_JOIN_ACTING){
			if(cmd->msg != 0){
				rc = join_acting(psu,cmd->msg);
				del_message_list(psu,sizeof(message_cmd_s));
			}
			else{
				/* TODO вернуть список игр */
				global_log("Пока не возврашаем список игр");
				del_message_list(psu,sizeof(message_cmd_s));
				psu->acting = 0;
			}
		}
		else{
			global_log("Некоректная комманда от игрока %s : %d",psu->name,psu->fd);
			del_user_list(psu->fd);
			return SUCCESS;
		}
	}

	rc = cmd_new_acting(psu->fd,psu->package,psu->acting);
	if(rc == FAILURE){
		global_log("Несмог отправить сообщение игроку %d на создание игры : %s",psu->fd,strerror(errno));
		del_user_list(psu->fd);
		return SUCCESS;
	}
	psu->package++;

	if(psu->acting == 0){
		global_log("Несмог присоединить игрока %s : %d к игре",psu->name,psu->fd);
	}
	else{
		global_log("Присоединил игрока %s : %d к игре %#04x",psu->name,psu->fd,psu->acting);
	}
	/*TODO запуск роботов*/

 	return SUCCESS;
}
/*****************************************************************************/
/* Основная функция                                                          */
/*****************************************************************************/
int init_list_acting(void)
{
	all_acting = g_hash_table_new_full(acting_hash,acting_equal,acting_destroy,NULL);
	global_log("Создал список игр!");
	return SUCCESS;
}

int deinit_list_acting(void)
{
	g_hash_table_destroy(all_acting);
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
		else{
			rc = check_bit_flag(flag,acting_user,1);
			if(rc == YES){
				continue;
			}
			else{
				rc = check_bit_flag(flag,message_user,1);
				if(rc == NO){
					continue;
				}
				else{
					rc = check_new_acting(ptu);
					if(rc == SUCCESS){
						(*success)--;
					}
				}
			}
		}
	}
	return SUCCESS;
}

int current_actings(void)
{
	return SUCCESS;
}

int delete_acting(uint16_t number)
{
	int rc;
	acting_s ta;
	acting_s * pta;
	/*user_s * ptu;*/
	ta.number = number;

	if(number == 0){
		global_log("нет такой игры 0");
		return FAILURE;
	}	

	rc = g_hash_table_lookup_extended(all_acting,(gpointer)&ta,(gpointer*)&pta,(gpointer*)&pta);
	if(rc == FALSE){
		global_log("Нет такой игры %#04x в списке",number);
	 	return FAILURE;
	}
	
	/*TODO послать команду другим участникам и удалить их*/

	g_hash_table_remove(all_acting,pta);
	global_log("Удаление игры под номером %#04x",number);
	
	return SUCCESS;
}
/*****************************************************************************/
