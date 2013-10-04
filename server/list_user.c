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
#include <string.h>
#include <assert.h>
#include <openssl/md5.h>
#include <time.h>
#include <stdint.h>

#include <glib.h>

#include "pub.h"
#include "log.h"
#include "protocol.h"

#include "kernel_pub.h"
#include "list_user_pub.h"

/*****************************************************************************/
/* Глобальные переменые                                                      */
/*****************************************************************************/
#define ADD_USER    100
static unsigned long int amount_user = 0;
static unsigned long int number_user = 0;
static user_s * begin_list_user = NULL;
static user_s * current_user = NULL;

/*****************************************************************************/
/* Основные функции                                                          */
/*****************************************************************************/
user_s * get_begin_user_list(void)
{
	return begin_list_user;
}

unsigned long int get_number_user(void)
{
	return number_user;
}

static int resize_list_user(void)
{
	unsigned long int size_new;
	unsigned long int size_old;
	user_s * ptu_new;
	user_s * ptu_old = begin_list_user;
	
	size_old = amount_user;
	/*TODO переделать добавление увеличивать в 2*/
	size_new = size_old + ADD_USER;
	size_old *= sizeof(user_s);
	size_new *= sizeof(user_s);
	
	ptu_new = (user_s*)malloc(size_new);	
		assert(ptu_new);
		
	memset(ptu_new,0,size_new);

	memcpy(ptu_new,ptu_old,size_old);

	free(ptu_old);

	begin_list_user = ptu_new;
	current_user = ptu_new + number_user;
	amount_user += ADD_USER;

	global_log("Увеличил список активных игроков на %d подсоединений!",amount_user);

	return SUCCESS;
}

int init_list_user(void)
{
	user_s * ptu;
	unsigned long int size;

	size = ADD_USER * sizeof(user_s);
	
	ptu = (user_s*)malloc(size);
		assert(ptu);
	
	amount_user = ADD_USER;	
	number_user = 0;
	memset(ptu,0,size);

	begin_list_user = ptu;
	current_user = ptu;

	global_log("Создал список активных игроков на %d подсоединений!",amount_user);
	return SUCCESS;	
}

int deinit_list_user(void)
{
	user_s * ptu = begin_list_user;

	ptu = begin_list_user;
	if(ptu != NULL)
		free(ptu);
	amount_user = 0;
	number_user = 0;

	global_log("Удалил список активных игроков!");
	return SUCCESS;
}

int add_user_list(int fd)
{
	char * temp;
	user_s * ptu;
	unsigned long int i;

	if(number_user == amount_user){
		resize_list_user();
	}
/*TODO проверить номера fd 0-stdin 1-stdout 2-stderr*/
	if(fd < 0){
		global_log("Некорректный идентификатор : %d!",fd);
		return FAILURE;
	}

	ptu = begin_list_user;
	for(i = 0;i < number_user;i++){
		if(fd == ptu->fd){
			global_log("Номера идентификаторов совпадают : %d!",fd);
			return FAILURE;
		}
		ptu++;
	}

	current_user->fd = fd;
	temp = current_user->name;
	memset(temp,0,LEN_USER_NAME);
	temp = (char*)current_user->passwd;
	memset(temp,0,MD5_DIGEST_LENGTH);
	current_user->timeout = time(NULL) + WAITING_USER;
	current_user->package = 0;
	current_user->buffer = g_byte_array_new ();

/*TODO преобразовать время*/	
	global_log("Соединения с сервером под номером %d время %ld!",current_user->fd,current_user->timeout);

	current_user++;
	number_user++;

	return SUCCESS;
}

int del_user_list(int fd)
{
	unsigned long int number;
	int size;
	user_s * ptu = begin_list_user;
	user_s * ptu_new = begin_list_user;
	user_s * ptu_old = begin_list_user;

	if(number_user == 0){
		global_log("В списке нет игроков!");
		return FAILURE;
	}
	for(number = 0;number < number_user;number++){
		if(fd == ptu->fd){
			global_log("Удаление из списка игрока %s под номером %d!",ptu->name,ptu->fd);
			break;
		}
		ptu++;
	}
	if(number == number_user){
		global_log("Нет такого индентификатора в списке : %d",fd);
		return FAILURE;
	}
	if((number + 1) != number_user){
		ptu_old += number;
		ptu_new += (number + 1);
			
		g_byte_array_free (ptu_old->buffer,TRUE);

		size = number_user - (number + 1);
		size *= sizeof(user_s);
		memmove(ptu_old,ptu_new,size);
	}
	
	current_user --;
	number_user --;
	return SUCCESS;
}
/*****************************************************************************/
