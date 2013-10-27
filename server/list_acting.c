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
#define AMOUNT_PLAYER      4
typedef struct _acting_s acting_s;
struct _acting_s
{
	uint16_t number;
	user_s * player[AMOUNT_PLAYER];
};

static GSList * begin_acting = NULL;
uint16_t amount_acting = UINT16_MAX;
/*****************************************************************************/
/* Вспомогательные функция                                                   */
/*****************************************************************************/
uint16_t set_number_acting(void)
{
	acting_s * pta;
	uint32_t i;
	GSList * current = begin_acting;
	for(i = UINT16_MAX;(i != 0) || (current != NULL);i--){
		pta = current->data;
		
	}
}

int unset_number_acting(uint16_t number)
{

}

static int create_acting(user_s * psu)
{
	uint32_t flag = psu->flag;
	int rc;
	acting_s * pta = NULL;

	rc = check_bit_flag(flag,robot_user);
	if(rc == YES){
		global_log("Робот не может создавать игру!");
		return FAILURE;
	}

	pta = g_slice_alloc0(sizeof(acting_s));
	pta->number = amount_acting;	
	set_bit_flag(flag,acting_user,1);
	psu->acting = pta->number;
	pta->player[0] = psu;
	rc = cmd_new_acting(psu->fd,psu->package,pta->number);
	if(rc == ){
		
	}		
	global_log("Создал новою игру : %d!",psu->fd);
	return  SUCCESS;
}

static int join_acting(user_s * psu)
{
	return SUCCESS;
}
static int check_new_acting(user_s * psu)
{
	message_cmd_s * msg;
	int rc;
	uint16_t flag;

	rc = read_message_list(psu,(uint8_t **)&msg);
	if(rc < sizeof(message_cmd_s)){
		return FAILURE;
	}
	
	if(msg->type == CMD_NEW_ACTING){
		rc = create_acting(psu);
		if(rc == )
		del_message_list(psu,rc);
	 	return SUCCESS;
	}

		  	&& (msg->type != CMD_JOIN_ACTING)){
	/*msg->type */

	/*TODO запуск роботов*/

	begin_acting = g_slist_prepend (begin_acting,pta);
	global_log("Создал новую игру : %d",pta->number);	
	amount_acting --;
	/*TODO проверка на заполниность играми*/
	if(amount_acting == 1){
		global_warning("Нет места для игр");
		return FAILURE;
	}
 	return SUCCESS;
}
/*****************************************************************************/
/* Основная функция                                                          */
/*****************************************************************************/
int init_list_acting(void)
{
	begin_acting = NULL;
	amount_acting = 0;
	global_log("Создал список игр!");
	return SUCCESS;
}

int deinit_list_acting(void)
{
	acting_s * pta;
	GSList * current = begin_acting;
	for(;current != NULL;){
		pta = current->data;
		g_slice_free1(sizeof(acting_s),pta);
		current = g_slist_next(current);
	}
	g_slist_free(begin_acting);
	begin_acting = NULL;
	amount_acting = 0;
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
/*****************************************************************************/
