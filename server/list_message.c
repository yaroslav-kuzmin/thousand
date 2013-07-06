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
#define AMOUNT_LISTS         3
#define SIZE_LIST_MESSAGE    4096
typedef struct _node_message_s node_message_s;
struct _node_message_s
{
	int free;
	unsigned char * begin;
	unsigned char * current;
};
int amount_node;
node_message_s * list_node = NULL;
/*****************************************************************************/
/* Вспомогательные функция                                                   */
/*****************************************************************************/

/*****************************************************************************/
/* Основные функции                                                          */
/*****************************************************************************/
/*Создает списки для хранения сообщений*/
int init_list_message(void)
{
	int i;
	size_t size = 0;
	node_messagw_s * t_node;
	unsigned char * tpucb;
	unsigned char * tpuc;

	amount_node = 1 << AMOUNT_LISTS;
	size = amount_node * sizeof(node_message_s);

	list_node = (node_messagw_s *)malloc(size);
		assert(list_node);
	size = amount_node * SIZE_LIST_MESSAGE;
	tpucb = (unsigned char *)malloc(size);
		assert(tpucb);
	
	t_node = list_node;
	tpuc = tpucb;

	for(i = 0;i < amount_node;i++){
		t_node->free = YES;
		t_node->begin = tpucb;
		t_node->current = tpuc;
		t_node++;
		tpuc += SIZE_LIST_MESSAGE;
	}

	return SUCCESS;
}

int reinit_list_message(void)
{
	
	return SUCCESS;
}

int deinit_list_message(void)
{
	int i;
	unsigned char * t;
	node_message_s * tb = list_node;
	for(i = 0;i < amount_node;i+= (1<<AMOUNT_LISTS)){
		t = tb->begin;
		free(t);
		tb += (1<<AMOUNT_LISTS); 
	}
	free(list_node);
	amount_node = 0;
	return SUCCESS;
}
/*Добавить список сообшений клиента*/
int add_list_message(user_s * psu)
{
	int i;
	node_message_s * t_node = list_node;
	unsigned char * t = NULL;
	for(i = 0;i < amount_node;i++){
		if(t_node->free == YES){
			t = t_node->begin;
			memset(t,0,SIZE_LIST_MESSAGE);
			psu->list_message =(all_message_u*)t;
			psu->first_message = (all_message_u*)t;
			psu->last_message = (all_message_u*)t;
			break;
		}
		t_node ++;
	}
	if(t == NULL){
	}

	return ;	
}
/*Удалить список сообщений клиента*/
int del_list_message(user_s * psu)
{
	void * buff = (void *)m;
	if( buff != NULL) 
		free(buff);
	return SUCCESS;
}	

static unsigned char temp_buff[SIZE_BUFF_MESSAGE];

int add_message_list(user_s * psu,unsigned char * buf,int len)
{
	unsigned char * t_new;
	unsigned char * t_old;
	all_message_u * begin = psu->list_message;
	all_message_u * first = psu->first_message;
	all_message_u * last = psu->last_message;
	all_message_u * msg = (all_message_u *)buf;
	int len_buff = len;
	int len_msg;
	int t_len;
	
	if(psu->partial == YES){
		t_new = temp_buff;
		t_old = psu->partial_buff;
		t_len = psu->len_partial;

		memset(t_new,0,SIZE_BUFF_MESSAGE);
		memcpy(t_new,t_old,t_len);
		t_new += t_len;
		/*TODO проверка на переполнение*/
		memcpy(t_new,buf,len_buff);
		msg = (all_message_u*)temp_buff;
		len_buff += t_len;
		psu->partial = NO'
		memset(t_old,0,SIZE_BUFF_PARTIAL);
	}

	len_msg = msg->len + (sizeof(message_cmd_s));

	for(;len_buff;){
		if(len_msg > len_buff){
		/*TODO проверка на переполнение*/
			t_new = psu->partial_buff;
			memcpy(t_new,buf,len_buff);
			psu->len_partial = len_buff;
			psu->partial = YES;
			break;
		}
		
	}
	
	return SUCCESS		
}	

int del_message_list(user_s * psu,all_message_u * msg)
{
	all_message_u * begin = psu->list_message;
	all_message_u * first = psu->first_message;
	all_message_u * last = psu->last_message;
	all_message_u * msg = NULL;

}	

/*****************************************************************************/
