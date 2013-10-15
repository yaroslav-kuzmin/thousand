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
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>
#include <signal.h>
#include <openssl/md5.h>
#include <stdint.h>

#include <glib.h>

#include "pub.h"
#include "total.h"
#include "warning.h"
#include "log.h"
#include "protocol.h"

#include "list_user.h"
/*****************************************************************************/
/* Глобальные переменые                                                      */
/*****************************************************************************/

/*****************************************************************************/
/* Основные функции                                                          */
/*****************************************************************************/
/*очередь ожидания*/
#define BACKLOG              10  
/*дискриптор  локального сокета*/
static int fd_local_socket = 0;  

int init_socket(void)
{
	int rc;
	int s_fd;
	struct sockaddr_un ssa;
	char * local_socket;

	s_fd = socket(PF_UNIX, SOCK_STREAM, 0);
	if(s_fd == -1){
		global_warning("Немогу создать локальный сокет : %s", strerror(errno));
		return FAILURE;
	}
	global_log("Создали сокет %#x!",s_fd);

	ssa.sun_family = AF_LOCAL;
	local_socket = get_local_socket();
	strcpy(ssa.sun_path,local_socket);
	
	rc = bind(s_fd,&ssa,SUN_LEN(&ssa));
	if(rc == -1){
		global_warning("Немогу связать локальный сокет с локальным файлом %s : %s",ssa.sun_path,strerror(errno));
		if(errno == EADDRINUSE){
			unlink(local_socket);
			rc = bind(s_fd,&ssa,SUN_LEN(&ssa));
			if(rc != -1){
				goto bind_success;
			}
		}
		close(s_fd);
		return FAILURE;
	}
bind_success:
	global_log("Соединили сокет %#x с файлом %s!",s_fd,ssa.sun_path);
	rc = listen(s_fd,BACKLOG);
	if(rc == -1){
		global_warning("Немогу создать очередь : %s",strerror(errno));
		close(s_fd);
		return FAILURE;
	}
	global_log("Ожидание запросов!");

	rc = fcntl(s_fd,F_SETFL,O_NONBLOCK|O_ASYNC); /*Установить флаги работа в неблокируюшем и асинхроным режиме*/
	if(rc == -1){
		global_warning("Несмог установить режим доступа к сокету %#x : %s",s_fd,strerror(errno));
		close(s_fd);
		return FAILURE;
	}
	rc = fcntl(s_fd , F_SETSIG , SIGIO);/*Установить сигнал, который будет послан, 
													  когда станет возможен ввод или вывод*/
	if(rc == -1){
		global_warning("Несмог привизать сигнал к сокету %#x : %s",s_fd,strerror(errno));
		close(s_fd);
		return FAILURE;
	}
	rc = fcntl(s_fd , F_SETOWN , getpid()); /*Установить идентификатор процесса или группу процесса, 
															которые будут принимать сигналы SIGIO и SIGURG для 
															событий на файловом дескрипторе */
	if(rc == -1){
		global_warning("Несмог установить привязку сигналов к процессу %#x : %#x : %s",getpid(),s_fd,strerror(errno));
		close(s_fd);
		return FAILURE;
	}
	global_log("Настроили сокет %#x!",s_fd);

	fd_local_socket = s_fd;
	return SUCCESS;	
}

int close_soket(void)
{
	char * name = get_local_socket();
	if(fd_local_socket != 0){
		close(fd_local_socket);
	}	
	fd_local_socket = 0;
	unlink(name);

	return SUCCESS;
}

int check_new_connect(void)
{
	struct sockaddr_un client_name;
	socklen_t client_name_len;
	int c_fd;
	int rc;
	int exit = FAILURE;

	for(;;){
		c_fd = accept(fd_local_socket,&client_name,&client_name_len);
		if(c_fd == -1){
			if(errno != EAGAIN){
				global_warning("Клиент не смог соединится : %s",strerror(errno));
			}
			break;
		}
		/*Установить флаги работа в неблокируюшем и асинхроным режиме*/
		rc = fcntl(c_fd,F_SETFL,O_NONBLOCK|O_ASYNC); 
		if( rc == -1){
			global_warning("Несмог установить режим доступа к клиенту %#x : %s",c_fd,strerror(errno));
			close(c_fd);
			continue;
		}
		/*Установить сигнал, который будет послан, когда станет возможен ввод или вывод*/
		rc = fcntl(c_fd , F_SETSIG , SIGIO);
		if(rc == -1){
			global_warning("Несмог привизать сигнал к сокету %#x : %s",c_fd,strerror(errno));
			close(c_fd);
			continue;
		}
		/*Установить идентификатор процесса или группу процесса, 
		которые будут принимать сигналы SIGIO и SIGURG для 
		событий на файловом дескрипторе */
		rc = fcntl(c_fd , F_SETOWN , getpid()); 
		if(rc == -1){
			global_warning("Несмог установить привязку сигналов к процессу %#x : %#x : %s",getpid(),c_fd,strerror(errno));
			close(c_fd);
			continue;
		}

		rc = add_user_list(c_fd);
		if(rc == FAILURE){
			close(c_fd);
		}
		else{
			exit = SUCCESS;
		}		
	}

	return exit;
}

/*****************************************************************************/
