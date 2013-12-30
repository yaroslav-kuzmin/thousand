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

int s_cmd_check_connect(user_s * psu);
int s_cmd_access_denied_login(user_s * psu);
int s_cmd_access_denied_passwd(user_s * psu);
int s_cmd_access_allowed(user_s * psu);
int s_cmd_new_acting(user_s * psu,uint16_t acting);
int s_cmd_join_acting(user_s * psu,uint16_t acting);
int s_cmd_join_player(user_s * psu,char * name);
int s_cmd_game_over(user_s * psu,uint16_t acting);
int s_cmd_number_round(user_s * psu,uint16_t round);
int s_cmd_amount_point_player(user_s * psu,uint8_t player,int16_t point);

#endif

/*****************************************************************************/

