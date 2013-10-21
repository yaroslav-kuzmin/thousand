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

#ifndef NET_SERVER_H
#define NET_SERVER_H

int init_socket(void);
int close_soket(void);
int check_new_connect(void);

int cmd_check_connect(int fd,uint16_t number);
int cmd_access_denied_login(int fd,uint16_t number);
int cmd_access_denied_passwd(int fd,uint16_t number);
int cmd_access_allowed(int fd,uint16_t number);

#endif

/*****************************************************************************/

