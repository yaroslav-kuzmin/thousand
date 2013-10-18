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

#ifndef LIST_MESSAGE_H
#define LIST_MESSAGE_H

uint32_t len_message_list(user_s * psu);
int write_message_list(user_s * psu,uint8_t * buf,int len);
/*вoщврашает обшие количество байт в буффере*/
int read_message_list(user_s * psu,uint8_t ** msg);
int del_message_list(user_s * psu,int len);

#endif

/*****************************************************************************/

