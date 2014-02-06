/*****************************************************************************/
/*                                                                           */
/*Thousand - card game                                                       */
/* Copyright (C) <2013> <Kuzmin Yaroslav>                                    */
/*                                                                           */
/* This program is free software: you can redistribute it and/or modify      */
/* it under the terms of the GNU General Public License as published by      */
/* the Free Software Foundation, either version 3 of the License, or         */
/* (at your option) any later version.                                       */
/*                                                                           */
/* This program is distributed in the hope that it will be useful,           */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of            */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             */
/* GNU General Public License for more details.                              */
/*                                                                           */
/* You should have received a copy of the GNU General Public License         */
/* along with this program.  If not, see <http://www.gnu.org/licenses/>.     */
/*                                                                           */
/*  Email contact: kuzmin.yaroslav@gmail.com                                 */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************/
/* Модуль пользовательского интерфейса                                       */
/*****************************************************************************/

#ifndef INTERFACE_H
#define INTERFACE_H

int if_get_name_player(char ** player,int len);
int if_set_name_player(char *player);
int if_get_passwd(char ** passwd,int len);
int if_set_passwd(char * passwd);
int if_set_connect(void);
int if_not_set_connetc(int type);
int if_new_game(void);
int if_create_game(uint16_t number);
int if_partner_left(char * name);
int if_partner_left_point(uint16_t point);
int if_partner_left_bolt(uint8_t bolt);
int if_partner_left_card(uint8_t card);
int if_partner_right(char * name);
int if_partner_right_point(uint16_t point);
int if_partner_right_bolt(uint8_t bolt);
int if_partner_right_card(uint8_t card);
int if_number_round(uint16_t round);
int if_status_round(status_round_e sr);
int if_player_point(uint16_t point);
int if_player_bolt(uint8_t bolt);
int if_card_player(uint8_t number_card,uint8_t card,uint8_t select);
int if_table(void);
int if_table_card_left(uint8_t card);
int if_table_card_center(uint8_t card);
int if_table_card_right(uint8_t card);
int if_clean_table(void);

interface_cmd_e if_cmd(void);
int if_nonblock(int non);

int init_interface(void);
int close_interface(void);

#endif

/*****************************************************************************/
