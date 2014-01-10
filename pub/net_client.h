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
/* Функции взаимодействия по сети                                            */
/*****************************************************************************/

#ifndef NET_CLIENT_H
#define NET_CLIENT_H

int init_socket(void);
int close_socket(void);
int read_socket(uint8_t ** buff,int len);
int write_socket(uint8_t * buff,int len);

int c_cmd_login(char * user);
int c_cmd_passwd(uint8_t * passwd);
int c_cmd_new_acting(void);
int c_cmd_join_acting(uint16_t number);
int c_answer_access_server(void);
int c_answer_new_acting(uint16_t * acting,uint8_t * player);
int c_answer_number_player(uint16_t * number);
int c_answer_join_acting(uint16_t * acting,uint8_t * player);
int c_answer_name_partner(uint8_t * number,char ** name);
int c_cmd_game_over(uint16_t acting);
int c_answer_message(all_message_u * msg);

#endif

/*****************************************************************************/

