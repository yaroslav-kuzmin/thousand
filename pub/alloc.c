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
/* Модуль выделения памяти для глобальных строк                              */
/* неизменых при выполнении програмы                                         */
/*****************************************************************************/
/*****************************************************************************/
/* Дополнительные файлы                                                      */
/*****************************************************************************/
#include <string.h>
#include <stdint.h>

#include <glib.h>

#include "pub.h"

/*****************************************************************************/
/* глобальные переменые                                                      */
/*****************************************************************************/
typedef struct line_s
{
	char * begin;
	char * position;
	uint32_t size;
	uint32_t item;
}line_t;

line_t * begin_line = NULL;
uint32_t amount_line = 0;

#define ADD_LINE    3
#define ADD_SIZE    4096

/*****************************************************************************/
/* Вспомогательные функция                                                   */
/*****************************************************************************/

int init_str_alloc(void)
{
	uint32_t size = sizeof(line_t);
	amount_line = ADD_LINE;
	size = amount_line * size;

	begin_line = (line_t *)g_malloc(size);

	memset(begin_line,0,size);

	return SUCCESS;
}

int deinit_str_alloc(void)
{
	int i;
	line_t * line;
	for(i = 0,line = begin_line;i < amount_line;i++,line++){
		char * t = line->begin;
		g_free(t);
	}
	g_free(begin_line);
	return SUCCESS;
}

int resize_line(void)
{
	line_t * line;
	uint32_t size = sizeof(line_t);
	uint32_t amount_temp = amount_line + ADD_LINE;
	size = amount_temp * size;

	line = (line_t *)g_malloc(size);

	memset(line,0,size);

	size = sizeof(line_t);
	size = amount_line * size;
	memcpy(line,begin_line,size);

	g_free(begin_line);
	begin_line = line;
	amount_line = amount_temp;

	return SUCCESS;
}

int new_line(line_t ** l,uint32_t s)
{
	uint32_t size;
	char * str;
	line_t * line = *l;

	size = ADD_SIZE;
	if( size < s){
		size = s + ADD_SIZE;
	}
	str = (char *) g_malloc(size);
	memset(str,0,size);

	line->begin = str;
	line->position = str;
	line->size = size;
	line->item = 0;

	return SUCCESS;
}

char * str_alloc(int add)
{
	int i;
	char * t;
	line_t * line;
	uint32_t size;
	uint32_t item;


	for(i = 0,line = begin_line;i < amount_line;i++,line++){
		size = line->size;
		if(size == 0){
			new_line(&line,add);
			goto exit_str_alloc;
		}
		item = line->item;
		if((item + add) < size){
			goto exit_str_alloc;
		}
	}

	resize_line();
	line = begin_line + i;
	new_line(&line,add);

exit_str_alloc:
	t = line->position;
	line->position += add;
	line->item += add;
	return t;
}
/*****************************************************************************/
