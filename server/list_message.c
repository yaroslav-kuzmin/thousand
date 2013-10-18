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
#include <openssl/md5.h>

#include <glib.h>

#include "pub.h"
#include "log.h"
#include "protocol.h"
#include "bit_flag.h"

#include "kernel_pub.h"
#include "list_user.h"

/*****************************************************************************/
/* Глобальные переменые                                                      */
/*****************************************************************************/

/*****************************************************************************/
/* Вспомогательные функция                                                   */
/*****************************************************************************/

/*****************************************************************************/
/* Основные функции                                                          */
/*****************************************************************************/
/*размер буфера */
uint32_t len_message_list(user_s * psu)
{
	GByteArray * array = psu->buffer;
	guint total_len = array->len;
	return total_len;
}

/* Записывает прищедшие сообщения в буфер*/
int write_message_list(user_s * psu,uint8_t * buf,int len)
{
	uint32_t flag = psu->flag;
	GByteArray * array = psu->buffer;

	array = g_byte_array_append(array,buf,len);
	set_bit_flag(flag,message_user,1); 

	psu->buffer = array;
	/*DEBUG*/
	g_message("wm : %d : %d",psu->fd,len);

	return SUCCESS;
}	
/* Читает сообщения из буфера без удаления*/
int read_message_list(user_s * psu,uint8_t ** msg)
{
	GByteArray * array = psu->buffer;
	guint total_len = array->len;

	*msg = array->data;
	return total_len;
}	
/* Удаляет сообщения из буфера */
int del_message_list(user_s * psu,int len)
{
	uint32_t flag = psu->flag;
	GByteArray * array = psu->buffer;
	
	/*DEBUG*/
	g_message("dm : %d : %d",psu->fd,len);

	array = g_byte_array_remove_range(array,0,len);
	if(array->len == 0){

		unset_bit_flag(flag,message_user,1);
	}

	psu->buffer = array;
	return SUCCESS;
}

/*****************************************************************************/
