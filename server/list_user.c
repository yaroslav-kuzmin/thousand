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
#include <string.h>
#include <assert.h>
#include <openssl/md5.h>
#include <time.h>
#include <stdint.h>
#include <unistd.h>

#include <glib.h>

#include "pub.h"
#include "log.h"
#include "protocol.h"
#include "bit_flag.h"

#include "list_user_pub.h"
#include "access.h"
#include "list_acting.h"

/*****************************************************************************/
/* Глобальные переменые                                                      */
/*****************************************************************************/
#define ADD_USER    100
/*static uint32_t amount_user = 0;*/
static GSList * begin_user = NULL;
static GSList * current_user = NULL;
/*****************************************************************************/
/* Вспомогательные функция                                                   */
/*****************************************************************************/
int compare_user_fd(gconstpointer a,gconstpointer b)
{
	int ta = ((user_s *)a)->fd;
	int tb = ((user_s *)b)->fd;
	if(ta == tb ){
		return SUCCESS;
	}
	return FAILURE;
}
/*****************************************************************************/
/* Основные функции                                                          */
/*****************************************************************************/
user_s * get_user_list(uint32_t number)
{
	return (user_s *)g_slist_nth_data(begin_user,number);
}

user_s * get_first_user_list(void)
{
	current_user = begin_user;
	if(current_user == NULL){
		return NULL;
	}
	return (user_s*)current_user->data;
}

user_s * get_next_user_list(void)
{
	current_user = g_slist_next(current_user);
	if(current_user == NULL){
		return NULL;
	}
	return (user_s*)current_user->data;
}

uint32_t get_amount_user(void)
{
	return g_slist_length(begin_user);
}
/*************************************/
int add_user_list(int fd)
{
	user_s * ptu;
	user_s tu;

	/*TODO проверить номера fd 0-stdin 1-stdout 2-stderr*/
	if(fd < 0){
		global_log("Некорректный идентификатор : %d!",fd);
		return FAILURE;
	}

	if(begin_user != NULL){
		tu.fd = fd;
		current_user = g_slist_find_custom(begin_user,&tu,compare_user_fd);
		if(current_user != NULL){
			global_log("Номера идентификаторов совпадают : %d!",fd);
			current_user = begin_user;
			return FAILURE;
		}
	}

	ptu = (user_s *)g_slice_alloc0(sizeof(user_s));

	ptu->fd = fd;
	ptu->flag = init_bit_flags(last_flag_user);
	/*ptu->name = {0};*/
	/*ptu->passwd = {0};*/
	ptu->timeout = time(NULL) + WAITING_USER;
	/*ptu->package = 0;*/
	ptu->buffer = g_byte_array_new ();

	begin_user = g_slist_prepend (begin_user,ptu);
	/*amount_user ++;*/
	current_user = begin_user;
/*TODO преобразовать время*/
	global_log("Соединения с сервером под номером %d время %ld!",ptu->fd,ptu->timeout);

	return SUCCESS;
}

int del_user_list(int fd)
{
	user_s * ptu;
	GByteArray * tb;
	uint32_t tf;
	uint16_t acting;
	user_s tu;

	if(begin_user == NULL){
		global_log("В списке нет игроков!");
		return FAILURE;
	}

	tu.fd = fd;
	current_user = g_slist_find_custom(begin_user,&tu,compare_user_fd);
	if(current_user == NULL){
		global_log("Нет такого идентификатора игрока : %d!",fd);
		current_user = begin_user;
		return FAILURE;
	}

	ptu = current_user->data;
	global_log("Удаление из списка игрока %s под номером %d!",ptu->name,ptu->fd);

	if(ptu->acting != 0){
		acting = ptu->acting;
		ptu->acting = 0;
		delete_acting(acting);
	}
	close(fd);
	close_access_user(ptu->name);
	tf = ptu->flag;
	deinit_bit_flag(tf);
	tb = ptu->buffer;
	g_byte_array_free(tb,TRUE);
	begin_user = g_slist_remove(begin_user,ptu);
	g_slice_free1(sizeof(user_s),ptu);
	/*amount_user --;*/
	current_user = begin_user;
	return SUCCESS;
}
/*************************************/
int init_list_user(void)
{
	begin_user = NULL;
	current_user = NULL;
	/*amount_user = 0;	*/
	global_log("Создал список активных игроков на подсоединений!");
	return SUCCESS;
}

int deinit_list_user(void)
{
	user_s * ptu;
	GByteArray * tb;
	current_user = begin_user;
	for(;current_user != NULL;){
		ptu = (user_s *)current_user->data;
		tb = ptu->buffer;
		g_byte_array_free(tb,TRUE);
		g_slice_free1(sizeof(user_s),ptu);
		current_user = g_slist_next(current_user);
	}
	g_slist_free(begin_user);
	begin_user = NULL;
	current_user = NULL;

	/*amount_user = 0;*/
	global_log("Удалил список активных игроков!");
	return SUCCESS;
}

/*****************************************************************************/
