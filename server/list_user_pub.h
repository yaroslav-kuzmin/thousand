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
/* Объявление глобальных макросов                                            */
/*****************************************************************************/

#ifndef LIST_USER_PUB_H
#define LIST_USER_PUB_H

#define ACCESS_USER(x)        (x & 0x01) 
#define SET_ACCESS_USER(x)    (x | 0x01)
#define UNSET_ACCESS_USER(x)  (x & 0xFE)

#define LOGIN_USER(x)
#define SET_LOGIN_USER(x)
#define UNSET_LOGIN_USER(x)

#define MESSAGE_USER(x)       (x & 0x02)
#define SET_MESSAGE_USER(x)   (x | 0x02)
#define UNSET_MESSAGE_USER(x) (x & 0xFD)

typedef struct _user_s user_s;

struct _user_s
{
	int fd;
	uint8_t flag;
	char name[LEN_USER_NAME];
	uint8_t passwd[MD5_DIGEST_LENGTH];
	time_t timeout;
	uint16_t package;
	GByteArray * buffer;
};

#endif

/*****************************************************************************/

