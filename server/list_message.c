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
#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <openssl/md5.h>

#include "pub.h"
#include "log.h"
#include "protocol.h"

#include "kernel_pub.h"
#include "list_user_pub.h"

/*****************************************************************************/
/* Глобальные переменые                                                      */
/*****************************************************************************/
#define AMOUNT_LISTS         (1<<2)   /*начальное колличество кратно степени 2*/
#define SIZE_BLOCK           4096    /*размер одного блока */

typedef struct _block_message_s block_message_s;
struct _block_message_s
{
	uint8_t * begin;
	uint8_t * current;
};
static uint32_t amount_block;
static block_message_s * list_block = NULL;
static int free_block = 0;
/*
static int amount_free_block = 0;
static uint8_t * free_block = NULL;
*/
static int size_temp_buff = 0;
static uint8_t * temp_buff;
/*****************************************************************************/
/* Вспомогательные функция                                                   */
/*****************************************************************************/
static int reinit_lists_message(void)
{
	int i;
	size_t size;
	uint8_t * tpucb;
	uint8_t * tpuc;
	int old_ab = amount_block;	
	int new_ab = amount_block << 1;
	block_message_s * t_block_old = list_block;
	block_message_s * t_block_new = NULL;

	size = new_ab * sizeof(block_message_s);	
	t_block_new = (block_message_s *)malloc(size);
		assert(t_block_new);
	memset(t_block_new,0,size);

	size = old_ab * sizeof(block_message_s);
	memcpy(t_block_new,t_block_old,size);
	free(list_block);
	list_block = t_block_new;

	size = (new_an - old_an) * SIZE_BLOCK;
	tpucb = (uint8_t *)malloc(size);
		assert(tpucb);
	memset(tpucb,0,size);
	tpuc = tpucb;

	t_block_new += old_an; 
	for(i = old_an;i < new_an;i++){
		t_block_new->begin = tpucb;
		t_block_new->current = tpuc;
		t_block_new ++;
		tpuc += SIZE_BLOCK; 
	}

	reinit_bit_flags(free_block,new_an); 
	amount_block = new_an;
	global_log("Увеличил Список Сообщений : %d!",amount_block);
	return SUCCESS;
}

static int increase_list_message(user_s * psu)
{
	uint8_t * begin = psu->list_message;
	uint8_t * first = psu->first_message;
	uint8_t * last = psu->last_message;
	uint8_t * end = psu->end_message;
	uint8_t * t_old = begin;
	uint8_t * t_new = NULL;
	int abu; /*количество блоков пользователя*/
	int i,j;
	int check;
	block_message_s * t_block;
	
	abu = (end - begin)/SIZE_BLOCK;
	abu ++;
	for(i = 0;i < amount_block;i++){
		check = check_bit_flag(free_block,i,abu);
		if(check == FAILURE){
			return FAILURE;
		}
		if(check == YES){
			t_block = list_block + i;
			t_old = t_block->begin;
			for(j = 0;j < abu;j++){
				t_block++;
				t_new = t_block->begin;
				if(t_old != t_new){
					check = NO;
					break;
				}
			}
			if(check == YES){
				t_block = list_block + i;
					
				break;
			}
		}

	}/*for(i = 0;i < amount_block;i++)*/


	return SUCCESS;
}

static int check_list_message(user_s * psu)
{
	uint8_t * begin = psu->list_message;
	uint8_t * first = psu->first_message;
	uint8_t * last = psu->last_message;
	uint8_t * end = psu->end_message;
	size_t size;

	if(begin == first){
		return SUCCESS;
	}
	if( (begin < first) && (first < last)){
		size = (size_t) last - (size_t) first;
		memmove(begin,first,size);
		psu->first_message = begin;
		psu->last_message = begin + size;
		return SUCCESS;
	}

	return SUCCESS;
}
/*****************************************************************************/
/* Основные функции                                                          */
/*****************************************************************************/
/*Создает списки для хранения сообщений*/
int init_lists_message(void)
{
	int i;
	size_t size = 0;
	block_message_s * t_block;
	uint8_t * tpucb;
	uint8_t * tpuc;

	amount_block = AMOUNT_LISTS;
	size = amount_block * sizeof(block_message_s);

	list_block = (block_message_s *)malloc(size);
		assert(list_block);
	size = amount_block * SIZE_BLOCK;
	tpucb = (uint8_t *)malloc(size);
		assert(tpucb);
	
	t_block = list_block;
	tpuc = tpucb;

	for(i = 0;i < amount_block;i++){
		t_block->begin = tpucb;
		t_block->current = tpuc;
		t_block++;
		tpuc += SIZE_BLOCK;
	}

	free_block = init_bit_flags(amount_block); 

	global_log("Инициализировал Список Сообщений колличеством %d!",amount_block);

	size_temp_buff = SIZE_BLOCK;
	temp_buff = malloc(size_temp_buff);
		assert(temp_buff);

	return SUCCESS;
}

int deinit_lists_message(void)
{
	unsigned int i;
	uint8_t * t;
	block_message_s * tb = list_block;

	for(i = AMOUNT_LISTS;i < amount_block; i = i<<1 ){
		t = tb->begin;
		free(t);
		tb = list_block + i; 
	}
	free(list_block);
	amount_block = 0;
	deinit_bit_flag(free_block);
	global_log("Удалил Список Сообщений!");
	return SUCCESS;
}

/*Добавить список сообшений клиента*/
int add_list_message(user_s * psu)
{
	uint32_t check;
	block_message_s * t_block = NULL;
	uint8_t * t = NULL;
	uint8_t *f_n = NULL;
iteration_add:	
	check = free_bit_flag(free_block,1);
	if(check == UINT32_MAX){
		reinit_lists_message();
		check = free_bit_flag(free_block,1);
		if(check == UINT32_MAX){
			global_warning("Несмог увеличеть размер буфера сообщений у пользователя %d!",psu->fd); 
			return FAILURE;
		}
	}
	t_block = list_block + check;
	t = t_block->current;
	memset(t,0,SIZE_BLOCK);
	psu->list_message = t;
	psu->first_message = t;
	psu->last_message = t;
	t = t + SIZE_BLOCK;
	psu->end_message = t;
	global_log("Добавил Список Сообщений Пользователю %d!",psu->fd);
	return SUCCESS;
}

/*Удалить список сообщений клиента*/
int del_list_message(user_s * psu)
{
	uint32_t i;
	uint8_t * t_check = NULL;
	uint8_t * t_begin = psu->list_message;
	uint8_t * t_end = psu->end_message;
	size_t size;
	block_message_s * t_block = list_block;

	for(i = 0;i < amount_block;i++){
		t_check = t_block->current;
		if(t_old == t_check){
			size = (t_end - t_begin)/SIZE_BLOCK;
			get_bit_flag(free_block,size);
			break;
		}
		t_block ++;
	}
	global_log("Удалил Список Сообщений Пользователя %d!",psu->fd);
	return SUCCESS;
}	

/*TODO возможна повышение производительности работы буфера если реализовать "круговой буфер"*/
/* Записывает прищедшие сообщения в буфер*/
int write_message_list(user_s * psu,uint8_t * buf,int len)
{

	uint8_t * t_new;
	uint8_t * t_old;
	uint8_t * begin = (uint8_t *)psu->list_message;
	uint8_t * first = (uint8_t *)psu->first_message;
	uint8_t * last = (uint8_t *)psu->last_message;
	uint8_t * end = (uint8_t *)psu->end_message;
	all_message_u * msg = (all_message_u *)buf;
	int len_buff = len;
	int len_msg;
	int t_len;
	
	if(psu->partial == YES){
		t_old = psu->partial_buff;
		t_len = psu->len_partial;
		if( size_temp_buff < (t_len + len_buff)){
			free(temp_buff);
			size_temp_buff = (t_len + len_buff );
			temp_buff = (uint8_t *)malloc(size_temp_buff);
				assert(temp_buff);
		}
		t_new = temp_buff;
		memset(t_new,0,size_temp_buff);
		memcpy(t_new,t_old,t_len);
		t_new += t_len;
		memcpy(t_new,buf,len_buff);

		msg = (all_message_u*)temp_buff;
		len_buff += t_len;

		psu->partial = NO;
		memset(t_old,0,SIZE_BUFF_PARTIAL);
	}

	for(;len_buff;){
		if(len_buff < (sizeof(message_cmd_s))){
			goto partial_exit;
		}

		len_msg = msg->cmd.field.len + (sizeof(message_cmd_s));

		if(len_msg > len_buff ){
			partial_exit;
		}

		t_new = (uint8_t *)msg;
		if((last == begin) || ((last + len_msg) < end)){
			memcpy(last,t_new,len_msg);
			last += len_msg;
			t_new += len_msg;
			msg = (all_message_u*)t_new;
			len_buff -= len_msg;
			break;
		}
		if((last + len_msg) > end){
			if(begin == first){

			}
		}

		if(len_buff < 0){
			global_warning("Некоректная обработка сообщений : длина Буфера сообщений равна %d",len_buff);
			break;
		}
	}
	goto exit_write_ok;
partial_exit:
	t_new = psu->partial_buff;
	/*TODO */
	if(len_buff > SIZE_BUFF_PARTIAL){
		global_warning("Переполнение буфера остаточных сообщений! - Данные потеряны");
		return FAILED;
	}
	memcpy(t_new,buf,len_buff);
	psu->len_partial = len_buff;
	psu->partial = YES;
exit_write_ok:
	return SUCCESS;
}	
/* Читает сообщения из буфера без удаления*/
int read_message_list(user_s * psu,uint8_t * msg)
{
	all_message_u * begin = psu->list_message;
	all_message_u * first = psu->first_message;
	all_message_u * last = psu->last_message;

	return SUCCESS;
}	
/* Удаляет сообщения из буфера */
int del_message_list(user_s * psu,int len)
{
}

/*****************************************************************************/
