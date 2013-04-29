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
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>
#include <signal.h>

#include "pub.h"
#include "total.h"
#include "warning.h"
#include "log.h"
#include "kernel.h"
/*****************************************************************************/
/* Глобальные переменые                                                      */
/*****************************************************************************/

/*****************************************************************************/
/* Основная функция                                                          */
/*****************************************************************************/
/*****************************************************************************/
/* Функции взаимодействия по сети                                            */
/*****************************************************************************/
#define BACKLOG              10 
int fd_local_socket = 0;

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
		global_warning("Несмог установить режим доступа к сокету %#x",s_fd);
		close(s_fd);
		return FAILURE;
	}
	rc = fcntl(s_fd , F_SETSIG , SIGIO);/*Установить сигнал, который будет послан, 
													  когда станет возможен ввод или вывод*/
	if(rc == -1){
		global_warning("Несмог привизать сигнал к сокету");
		close(s_fd);
		return FAILURE;
	}
	rc = fcntl(s_fd , F_SETOWN , getpid()); /*Установить идентификатор процесса или группу процесса, 
															которые будут принимать сигналы SIGIO и SIGURG для 
															событий на файловом дескрипторе */
	if(rc == -1){
		global_warning("Несмог установить привязку сигналов к процессу %#x",getpid());
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
	close(fd_local_socket);
	unlink(name);

	return SUCCESS;
}

#define SIZE_BUFF  1024
int read_sockect(int fd)
{
	char 	buff[SIZE_BUFF];
	int rc;

	//for(;;){
		memset(buff,0,SIZE_BUFF);
		rc = recv(fd,buff,SIZE_BUFF,0);
		if(rc == -1){
			if(errno != EAGAIN){
				global_warning("Ошибка чтения сокета : %s",strerror(errno));
			}
		}
		DEBUG_PRINTF_S(buff);
		rc = strcmp(buff,"quit");
		if(rc == 0 ){
			return SUCCESS;
		}
		/*
		if(amount_sig_io <= 1){	
			amount_sig_io = 0;
			rc = pause();
		}
		else{
			amount_sig_io--;
		}
	}*/

	memset(buff,0,SIZE_BUFF);
	strcat(buff,"test send");
	rc = strlen(buff);
	DEBUG_PRINTF_S(buff);
	send(fd,buff,rc,0);
	return FAILURE;
}

int connect_socket(void)
{
	struct sockaddr_un client_name;
	socklen_t client_name_len;
	int client_socket_fd;
	int rc;

	for(;;){
		client_socket_fd = accept(fd_local_socket,&client_name,&client_name_len);
		if(client_socket_fd == -1){
			if(errno != EAGAIN){
				global_warning("Клиент не смог соединится : %s",strerror(errno));
			}
		}
		else{
			rc = read_sockect(client_socket_fd);
			if(rc == SUCCESS){
				break;
			}
		}
			
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
