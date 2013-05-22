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
#include <assert.h>
#include <openssl/md5.h>
#include <time.h>

#include "pub.h"

/*****************************************************************************/
/* Глобальные переменые                                                      */
/*****************************************************************************/
#define LEN_USER_NAME      64

typedef struct user_s
{
	char name[LEN_USER_NAME];
	unsigned char passwd[MD5_DIGEST_LENGTH];
	int number_fd;
	time_t timeout;
}user_t;

#define ADD_USER    3
unsigned long int amount_user = 0;
user_t * begin_list_user = NULL;
user_t * current_user = NULL;

/*****************************************************************************/
/* Основные функции                                                          */
/*****************************************************************************/

int resize_list_user(void)
{
	unsigned long int size;
	user_t * ptu_new;
	user_t * ptu_old = begin_list_user;

}

int init_list_user(void)
{
	user_t * ptu;
	unsigned long int size;

	size = ADD_USER * sizeof(user_t);
	
	ptu = (user_t*)malloc(size);
		assert(ptu);
	
	amount_user = ADD_USER;	
	memset(ptu,0,size);

	begin_list_user = ptu;
	current_user = ptu;
			
	return SUCCESS;	
}

int deinit_list_user(void)
{

	return SUCCESS;
}
/*****************************************************************************/
