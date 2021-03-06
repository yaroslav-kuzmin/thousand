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
#include <sys/time.h>
#include <string.h>

#include <glib.h>

#include "pub.h"
#include "warning.h"
#include "log.h"
#include "protocol.h"

#include "list_user.h"
#include "net_server.h"
#include "list_message.h"
#include "access.h"
#include "list_acting.h"
/*****************************************************************************/
/* Глобальные переменые                                                      */
/*****************************************************************************/

/*****************************************************************************/
/* Вспомогательные функции                                                   */
/*****************************************************************************/

/*****************************************************************************/
/* Основные функции                                                          */
/*****************************************************************************/
volatile sig_atomic_t amount_alrm = 0;
void handler_timer(int signal_num)
{
	amount_alrm++;
	/*g_message("alarm : %d",amount_alrm);*/
}
volatile sig_atomic_t amount_sig_io = 1;
void sigaction_io(int num,siginfo_t * sig,void * test)
{
	amount_sig_io++;
	/*TODO более точная обработка дискрипторов*/
	/*g_message("fd :> %d : sig_io :> %d",sig->si_fd,amount_sig_io);*/
}

struct itimerval main_timer;
int set_timer(void)
{
 	int rc ;
	main_timer.it_interval.tv_sec = WAITING_USER;
	main_timer.it_interval.tv_usec = 0;
	main_timer.it_value.tv_sec = WAITING_USER;
	main_timer.it_value.tv_usec = 0;

	rc = setitimer(ITIMER_REAL,&main_timer,NULL);
	if(rc == -1){
		global_warning("Несмог запустить таймер : %s",strerror(errno));
		rc = FAILURE;
	}
	else{
		rc = SUCCESS;
	}
	return rc;
}

#define SIZE_TEMP_BUFF    8192
unsigned char t_buff[SIZE_TEMP_BUFF];

int main_loop(void)
{
	int rc;
	user_s * ptu;
	int fd;
	int new_connect = NO;
	int new_acting = 0;

	for(;;){
g_message("New check !");
/*Проверка нового подсоединения*/
		rc = check_new_connect();
		if(new_connect == NO){
			new_connect = rc;
		}
/*Чтение информации от клиентов*/
		ptu = get_first_user_list();
		for(;ptu != NULL;ptu = get_next_user_list()){
			fd = ptu->fd;
			rc = recv(fd,t_buff,SIZE_TEMP_BUFF,0);
			if(rc > 0){
/*g_message(" read fd : %d | rc : %d",fd,rc);*/
				write_message_list(ptu,t_buff,rc);
			}
			else{
			/*данных от клиента не поступало*/
				if(rc == -1 ){
					if(errno == EAGAIN){
						goto check_timeout;
					}
					else{
						/*TODO проверка ошибки*/
						global_log("Ошибка в соединении : %d : %s",fd,strerror(errno));
						/*TODO удаляется игрок из списка и указатель списка переходит в начало (( */
						del_user_list(fd,NOT_ACTING_DEL);
						/*TODO Проверка сушествующих игр на корректность*/
						continue;
					}
				}
				if(rc == 0){
					/*Если канал был закрыт но непришло сообщение о прекрашении работы*/
check_timeout:
					if(ptu->timeout <= time(NULL)){
						rc = s_cmd_check_connect(ptu);
						if(rc == FAILURE){
						/*TODO корректное сохранение игры */
						/*TODO проверка ошибки отправки сообщения*/
						/*TODO удаляется игрок из списка и указатель списка переходит в начало (( */
							del_user_list(fd,NOT_ACTING_DEL);
							continue;
						}
						ptu->timeout = time(NULL) + WAITING_USER;
					}
				}
			}
		}

		if(new_connect == YES){
			rc = access_users(&new_acting);
			if(rc == SUCCESS){
				new_connect = NO;
			}
		}
		/*Создание новый игры или присоединение к имеющейся игре*/
		if(new_acting > 0){
			create_actings(&new_acting);
			if(new_acting < 0){
				new_acting = 0;
			}
		}

		current_actings();

/* Ожидание сигналов на дискрипторах*/
/*TODO */
		if(amount_sig_io <= 1){
			amount_sig_io = 0;
/*g_message("Pause!");*/
			rc = pause();
		}
		else{
			amount_sig_io--;
			/*TODO взвести таймер с начала*/
		}
	}/*main loop*/

	return SUCCESS;
}
/*****************************************************************************/
