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

/*****************************************************************************/
/* Дополнительные файлы                                                      */
/*****************************************************************************/

#include <stdlib.h>
#include <stdint.h>
#include <openssl/md5.h>

#include <glib.h>

#include "pub.h"
#include "log.h"
#include "cards.h"
#include "protocol.h"
#include "net_client.h"

/*****************************************************************************/
/* Глобальные переменые                                                      */
/*****************************************************************************/

uint8_t number_robot = 0;
#define MIN_LEVEL_MASTER  0
#define MAX_LEVEL_MASTER  5
int master_robot = MIN_LEVEL_MASTER;

#if 1
/*Отладка*/
static char black_hearts[4]   = {0xe2,0x99,0xa5,0};
static char black_diamonds[4] = {0xe2,0x99,0xa6,0};
static char black_clubs[4]    = {0xe2,0x99,0xa3,0};
static char black_spades[4]   = {0xe2,0x99,0xa0,0};
static char ace[3] = " A";
static char ten[3] = "10";
static char king[3] = " K";
static char queen[3] = " Q";
static char jack[3] = " J";
static char nine[3] = " 9";
static char shirt_suit[4] = {0xe2,0x96,0x92,0};
static char shirt_value[5] = {0x20,0xe2,0x96,0x92,0};
#endif

uint16_t max_bet = AUTOMAT_BETS;

/*****************************************************************************/
/* Вспомогательные функция                                                   */
/*****************************************************************************/

/*****************************************************************************/
/* Основная функция                                                          */
/*****************************************************************************/
int ai_set_begin_round(int number)
{
	global_log("Номер раунда : %d",number);
	max_bet = AUTOMAT_BETS;
	return SUCCESS;
}

int ai_set_player_point(uint8_t player,int16_t point,uint8_t bolt)
{
	global_log("Игрок %d : очков %d : болтов %d",player,point,bolt);
  	/*TODO сохронять информацию для более профессиональной игры*/
	return SUCCESS;
}

int ai_set_status_player(uint8_t number_player,uint8_t status_player)
{
	global_log("Игрок %d : статус %d",number_player,status_player);
	return SUCCESS;
}
#if 1
static char * check_suit(uint8_t suit)
{
	char * str = shirt_suit;
	switch(suit){
		case HEARTS:
			str = black_hearts;
			break;
		case DIAMONDS:
			str = black_diamonds;
			break;
		case CLUBS:
			str = black_clubs;
			break;
		case SPADES:
			str = black_spades;
			break;
	}
	return str;
}
static char * check_value(uint8_t value)
{
	char * str = shirt_value;
	switch(value){
		case ACE:
			str = ace;
			break;
		case TEN:
			str = ten;
			break;
		case KING:
			str = king;
			break;
		case QUEEN:
			str = queen;
			break;
		case JACK:
			str = jack;
			break;
		case NINE:
			str = nine;
			break;
	}
	return str;
}
#endif
int ai_set_card(int amount,uint8_t * cards)
{
	int i;
	char * str_suit = NULL;
	char * str_value = NULL;

	global_log("Карты ");
	for(i = 0;i < amount;i++){
		uint8_t card = cards[i];
		str_suit = check_suit(SUIT_CARD(card));
		str_value = check_value(VALUE_CARD(card));
		global_log(" %s %s",str_suit,str_value);
	}
	return SUCCESS;
}

int ai_set_auction(uint16_t bet)
{
	if(bet == PASS_BETS){
		global_log("Торги : спасовал");
		return SUCCESS;
	}
	if(bet == WAIT_BETS){
		global_log("Торги : ожидаю хода");
		return SUCCESS;
	}
#if 1
/*отладка*/
	if(bet == AUTOMAT_BETS){
		c_answer_auction((AUTOMAT_BETS + MIN_ADD_BETS));
		global_log("Торги : сделал ставку %d",(AUTOMAT_BETS + MIN_ADD_BETS));
	}
	if(bet == (AUTOMAT_BETS + MIN_ADD_BETS)){
		c_answer_auction((AUTOMAT_BETS + (2 * MIN_ADD_BETS)));
		global_log("Торги : сделал ставку %d",(AUTOMAT_BETS + (2*MIN_ADD_BETS)));
	}
	if(bet >= (AUTOMAT_BETS + (2*MIN_ADD_BETS))){
		c_answer_auction(PASS_BETS);
		global_log("Торги : спасовал");
	}
#endif

	return SUCCESS;
}

int ai_set_player_bets(uint8_t player,uint16_t bet)
{
	global_log("Игрок %d : ставка %d",player,bet);
	return SUCCESS;
}

int ai_set_master_robot(char * str_master)
{
	int i = atoi(str_master);
	if( i < MIN_LEVEL_MASTER || i > MAX_LEVEL_MASTER ){
		master_robot = MIN_LEVEL_MASTER;
	}
	master_robot = i;
	global_log("Уровень робота : %d",master_robot);
	return SUCCESS;
}
/*****************************************************************************/
