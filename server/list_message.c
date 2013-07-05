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
#include "pub.h"
#include "protocol.h"
/*****************************************************************************/
/* Глобальные переменые                                                      */
/*****************************************************************************/
#define SIZE_LIST_MESSAGE    4096
/*****************************************************************************/
/* Вспомогательные функция                                                   */
/*****************************************************************************/

/*****************************************************************************/
/* Основная функция                                                          */
/*****************************************************************************/
/*TODO Переделать блок выделения памяти*/
all_message_u * init_list_message(void)
{
	void * buff = NULL;

	buff = malloc(SIZE_LIST_MESSAGE);
		assert(buff);
	return (all_message_u *)buff;	
}

int deinit_list_message(all_message_u * m)
{
	void * buff = (void *)m;
	if( buff != NULL) 
		free(buff);
	return SUCCESS;
}	

int add_message_list(user_s * psu,unsigned char * buf,int len)
{
	all_message_u * begin = psu->list_message;
	all_message_u * first = psu->first_message;
	all_message_u * last = psu->last_message;
	all_message_u * msg = (all_message_u *)buf;
		
			
}	

int del_message_list(user_s * psu,unsigned char * buf,int len)
{
	all_message_u * begin = psu->list_message;
	all_message_u * first = psu->first_message;
	all_message_u * last = psu->last_message;
	all_message_u * msg = NULL;

}	

/*****************************************************************************/
