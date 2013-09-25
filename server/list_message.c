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
#define AMOUNT_BIT_IN_BYTE     3
typedef struct _node_message_s node_message_s;
struct _node_message_s
{
	unsigned char * begin;
	unsigned char * current;
};
static int amount_node;
static node_message_s * list_node = NULL;
static free_node = 0;
/*
static int amount_free_node = 0;
static unsigned char * free_node = NULL;
*/
static int size_temp_buff = 0;
static unsigned char * temp_buff;
/*****************************************************************************/
/* Вспомогательные функция                                                   */
/*****************************************************************************/
static int reinit_lists_message(void)
{
	int i;
	size_t size;
	unsigned char * tpucb;
	unsigned char * tpuc;
	int old_an = amount_node;	
	int new_an = amount_node << 1;
	node_message_s * t_node_old = list_node;
	node_message_s * t_node_new = NULL;

	size = new_an * sizeof(node_message_s);	
	t_node_new = (node_message_s *)malloc(size);
		assert(t_node_new);
	memset(t_node_new,0,size);

	size = old_an * sizeof(node_message_s);
	memcpy(t_node_new,t_node_old,size);
	free(t_node_old);

	size = (new_an - old_an) * SIZE_BUFF_MESSAGE;
	tpucb = (unsigned char *)malloc(size);
		assert(tpucb);
	memset(tpucb,0,size);
	tpuc = tpucb;

	list_node = t_node_new;

	t_node_new += old_an; 
	for(i = old_an;i < new_an;i++){
		t_node_new->begin = tpucb;
		t_node_new->current = tpuc;
		t_node_new ++;
		tpuc += SIZE_BUFF_MESSAGE; 
	}

	reinit_bit_flags(free_node,new_an); 
	amount_node = new_an;
	global_log("Увеличил Список Сообщений : %d!",amount_node);
	return SUCCESS;
}

static int increase_list_message(user_s * psu)
{
	unsigned char * begin = (unsigned char *)psu->list_message;
	unsigned char * first = (unsigned char *)psu->first_message;
	unsigned char * last = (unsigned char *)psu->last_message;
	unsigned char * end = (unsigned char *)psu->end_message;
	unsigned char * t_old = begin;
	unsigned char * t_new = NULL;
	int size;
	int i,j;
	node_message_s * t_node;
	
	size = (end - begin)/SIZE_BUFF_MESSAGE;
	t_node = list_node;
	for(i = 0;i < amount_node;i++){
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
	node_message_s * t_node;
	unsigned char * tpucb;
	unsigned char * tpuc;
	unsigned long int * tpul;

	amount_node = AMOUNT_LISTS;
	size = amount_node * sizeof(node_message_s);

	list_node = (node_message_s *)malloc(size);
		assert(list_node);
	size = amount_node * SIZE_BUFF_MESSAGE;
	tpucb = (unsigned char *)malloc(size);
		assert(tpucb);
	
	t_node = list_node;
	tpuc = tpucb;

	for(i = 0;i < amount_node;i++){
		t_node->begin = tpucb;
		t_node->current = tpuc;
		t_node++;
		tpuc += SIZE_BUFF_MESSAGE;
	}

	free_node = init_bit_flags(amount_node); 

	global_log("Инициализировал Список Сообщений колличеством %d!",amount_node);

	size_temp_buff = SIZE_BUFF_MESSAGE;
	temp_buff = malloc(size_temp_buff);
		assert(temp_buff);

	return SUCCESS;
}

int deinit_lists_message(void)
{
	unsigned int i;
	unsigned char * t;
	node_message_s * tb = list_node;

	for(i = AMOUNT_LISTS;i < amount_node; i = i<<1 ){
		t = tb->begin;
		free(t);
		tb = list_node + i; 
	}
	free(list_node);
	amount_node = 0;
	deinit_bit_flag(free_node);
	global_log("Удалил Список Сообщений!");
	return SUCCESS;
}
/*Добавить список сообшений клиента*/
int add_list_message(user_s * psu)
{
	uint32_t check;
	node_message_s * t_node = NULL;
	unsigned char * t = NULL;
	unsigned char *f_n = NULL;
iteration_add:	
	check = free_bit_flag(free_node,1);
	if(check == UINT32_MAX){
		reinit_lists_message();
		check = free_bit_flag(free_node,1);
	}
	t_node = list_node + check;
	t = t_node->current;
	memset(t,0,SIZE_BUFF_MESSAGE);
	psu->list_message =(all_message_u*)t;
	psu->first_message = (all_message_u*)t;
	psu->last_message = (all_message_u*)t;
	t_node++;
	t = t_node->current;
	psu->end_message = (all_message_u*)t;
	global_log("Добавил Список Сообщений Пользователю %d!",psu->fd);
	return SUCCESS;
}

/*Удалить список сообщений клиента*/
int del_list_message(user_s * psu)
{
	uint32_t i,s;
	unsigned char * f_n;
	unsigned char * t_check = NULL;
	unsigned char * t_old = (unsigned char *)psu->list_message;
	unsigned char * 
	node_message_s * t_node = list_node;

	for(i = 0;i < amount_node;i++){
		t_check = t_node->current;
		if(t_old == t_check){
			
			get_bit_flag(free_node,i);
			break;
		}
		t_node ++;
	}
	global_log("Удалил Список Сообщений Пользователя %d!",psu->fd);
	return SUCCESS;
}	

int write_message_list(user_s * psu,unsigned char * buf,int len)
{

	unsigned char * t_new;
	unsigned char * t_old;
	unsigned char * begin = (unsigned char *)psu->list_message;
	unsigned char * first = (unsigned char *)psu->first_message;
	unsigned char * last = (unsigned char *)psu->last_message;
	unsigned char * end = (unsigned char *)psu->end_message;
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
			temp_buff = (unsigned char *)malloc(size_temp_buff);
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

		t_new = (unsigned char *)msg;
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

int read_message_list(user_s * psu,all_message_u * msg)
{
	all_message_u * begin = psu->list_message;
	all_message_u * first = psu->first_message;
	all_message_u * last = psu->last_message;

	return SUCCESS;
}	

/*****************************************************************************/
