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
/* Объявление глобальных макросов                                            */
/*****************************************************************************/

#ifndef LIST_USER_PUB_H
#define LIST_USER_PUB_H

#define LEN_USER_NAME      64
#define WAITING_USER       100  /*Проверка подключения в секундах*/ 

typedef struct user_s
{
	int number_fd;
	char name[LEN_USER_NAME];
	unsigned char passwd[MD5_DIGEST_LENGTH];
	time_t timeout;
}user_t;

#endif

/*****************************************************************************/
