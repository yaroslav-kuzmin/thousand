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
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <stdint.h>
#include <openssl/md5.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>

#include <glib.h>

#include "pub.h"
#include "protocol.h"

#include "kernel_pub.h"
#include "net_server.h"
#include "list_user.h"
#include "list_message.h"
#include "access.h"
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
	int fd;
	int new_connect = FAILURE;


	g_message("main_loop");

	for(;;){
/*Проверка нового подсоединения*/
		new_connect = check_new_connect();
/*Чтение информации от клиентов*/
		ptu = get_first_user_list();
		for(;ptu != NULL;){
			fd = ptu->fd;
			rc = recv(fd,t_buff,SIZE_TEMP_BUFF,0);
			if(rc == -1){
			/*данных от клиента не поступало*/	
				if(errno != EAGAIN){
					/* TODO проверка ошибки*/
					/*TODO удаляется игрок из списка и указатель списка переходит в начало (( */
					del_user_list(fd); 
					continue;
				}
				if(ptu->timeout <= time(NULL)){
					rc = cmd_check_connect(ptu->fd,ptu->package);
					if(rc == -1){
					/*TODO корректное сохранение игры */
					/*TODO проверка ошибки отправки сообщения*/	
					/*TODO удаляется игрок из списка и указатель списка переходит в начало (( */
						del_user_list(fd);
						continue;
					}
					ptu->package ++;
					ptu->timeout = time(NULL) + WAITING_USER;
				}
				ptu = get_next_user_list();
				continue;
			}
			write_message_list(ptu,t_buff,rc);
			ptu = get_next_user_list();
		}

		if(new_connect == SUCCESS){
			new_connect = access_user();
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
	
	return SUCCESS;
}
/*****************************************************************************/
