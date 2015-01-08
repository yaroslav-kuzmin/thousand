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
/* Модуль ai : анализ игры                                                   */
/*****************************************************************************/

#ifndef AI_H
#define AI_H

extern uint8_t number_robot;

int ai_set_begin_round(int number);
int ai_set_player_point(uint8_t player,int16_t point,uint8_t bolt);
int ai_set_status_player(uint8_t number_player,uint8_t status_player);
int ai_set_card(int amount,uint8_t * cards);
int ai_set_auction(uint16_t bet);
int ai_set_player_bets(uint8_t player,uint16_t bet);
int ai_set_master_robot(char * str_master);
#endif

/*****************************************************************************/
