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
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <openssl/md5.h>

#include "pub.h"
#include "protocol.h"

#include "kernel_pub.h"
#include "net_server.h"
#include "protocol.h"
#include "list_user_pub.h"
#include "list_user.h"
#include "list_message.h"
/*****************************************************************************/
/* Глобальные переменые                                                      */
/*****************************************************************************/

/*****************************************************************************/
/* Вспомогательные функции                                                   */
/*****************************************************************************/
/* TODO
volatile sig_atomic_t amount_alrm = 0;
void handler_timer(int signal_num)
{
	amount_alrm++;
}
*/
volatile sig_atomic_t amount_sig_io = 1;
void sigaction_io(int num,siginfo_t * sig,void * test)
{
	amount_sig_io++;
}

/*****************************************************************************/
/* Основная функция                                                          */
/*****************************************************************************/
#define SIZE_TEMP_BUFF    8192
unsigned char t_buff[SIZE_TEMP_BUFF];

int main_loop(void)
{
	int rc;
	user_s * ptu;
	unsigned long int nu,i;
	unsigned char buf;

	user_s  ptu0;
	user_s  ptu1;
	user_s  ptu2;
	user_s  ptu3;
	user_s  ptu4;
	user_s  ptu5;
	user_s  ptu6;
	user_s  ptu7;
	user_s  ptu8;
	user_s  ptu9;

	ptu0.fd = 0;
	ptu1.fd = 1;
	ptu2.fd = 2;
	ptu3.fd = 3;
	ptu4.fd = 4;
	ptu5.fd = 5;
	ptu6.fd = 6;
	ptu7.fd = 7;
	ptu8.fd = 8;
	ptu9.fd = 9;

	DEBUG_PRINTF_S("main_loop");
	
	add_list_message(&ptu0);
	add_list_message(&ptu1);

	add_list_message(&ptu4);
	del_list_message(&ptu4);

	add_list_message(&ptu2);
	add_list_message(&ptu3);
	add_list_message(&ptu5);
	add_list_message(&ptu6);

	del_list_message(&ptu6);
	del_list_message(&ptu3);
	del_list_message(&ptu5);

	add_list_message(&ptu7);
	add_list_message(&ptu8);
	add_list_message(&ptu9);
	del_list_message(&ptu9);

	del_list_message(&ptu0);
	del_list_message(&ptu1);
	del_list_message(&ptu2);
	del_list_message(&ptu7);
	del_list_message(&ptu8);
#if 0
	for(;;){
/*Проверка нового подсоединения*/
		check_new_connect(); 
/*Чтение информации от клиентов*/
		ptu = get_begin_user_list();
		nu = get_number_user()
		for(i = 0;i < nu;i++){
			fd = ptu->fd;
			rc = recv(fd,t_buff,SIZE_BUFF_MESSAGE,0);
			if(rc == -1){
			/*данных от клиента не поступало*/	
				if(errno != EAGAIN){
				/* TODO проверка ошибки*/
					del_user_list(fd);
					nu = get_number_user();
					continue;
				}
				if(ptu->timeout <= time(NULL)){
					message_cmd_u temp;					
					temp.msg.type = CMD_CHECK_CONNECT;
					temp.msg.len = 0;
					rc = send(fd,temp.buf,(sizeof(message_cmd_s)),0);
					if(rc == -1){
					/*TODO корректное сохранение игры */
					/*TODO проверка ошибки отправки сообщения*/	
						del_user_list(fd);
						nu = get_number_user();
						continue;
					}
					ptu->timeout = time(NULL) + WAITING_USER;
				}
				ptu++;
				continue;
			}
			/*TODO проверка на переполнение буфера при приеме сообщения и неполном приходе данных */
			(ptu,t_buff,rc);
			ptu++;
		}
/* Ожидание сигналов на дискрипторах*/
		if(amount_sig_io <= 1){	
			amount_sig_io = 0;
			rc = pause();
		}
		else{
			amount_sig_io--;
		}
	}
#endif
	return SUCCESS;
}
/*****************************************************************************/
