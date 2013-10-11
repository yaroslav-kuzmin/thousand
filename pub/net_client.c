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
/* Функции взаимодействия по сети                                            */
/*****************************************************************************/
/*****************************************************************************/
/* Дополнительные файлы                                                      */
/*****************************************************************************/
#include <stdint.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "pub.h"
#include "total.h"
#include "warning.h"
#include "log.h"

/*****************************************************************************/
/* глобальные переменые                                                      */
/*****************************************************************************/

/*****************************************************************************/
/* Вспомогательные функция                                                   */
/*****************************************************************************/

static int fd_local_socket;

int init_socket(void)
{
	struct sockaddr_un ssa;
	char * local_socket;
	int rc;

	fd_local_socket = socket(PF_UNIX, SOCK_STREAM, 0);
	if(fd_local_socket == -1){
		global_warning("Немогу создать локальный сокет : %s", strerror(errno));
		return FAILURE;
	}
	global_log("Создали сокет %#x",fd_local_socket);
	ssa.sun_family = AF_LOCAL;
	local_socket = get_local_socket();
	strcpy(ssa.sun_path,local_socket);
	rc = connect(fd_local_socket,&ssa,SUN_LEN(&ssa));
	if(rc == -1){
		global_warning("Несмог соединится с локальным сокетом : %s",ssa.sun_path);
		close(fd_local_socket);
		return FAILURE;
	}
	global_log("Соединились с сервером!");

	return SUCCESS;
}

int write_socket(uint8_t * buff,int len)
{
	int rc;
	rc = send(fd_local_socket, buff , len , 0);
	/*DEBUG*/
	global_log("Отправил на сервер %s",buff);

	return rc;
}

int read_socket(uint8_t ** buff,int len)
{
	uint8_t * tbuff = *buff;
	int rc = recv(fd_local_socket,tbuff,len,0);
	return rc;
}

int close_socket(void)
{
	close(fd_local_socket);
	return SUCCESS;
}
/*****************************************************************************/
