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
/* Модуль decks - генерирует колоды карт для игры                            */
/*****************************************************************************/

/*****************************************************************************/
/* Дополнительные файлы                                                      */
/*****************************************************************************/
#include <stdint.h>

#include <glib.h>

#include "pub.h"
#include "cards.h"

/*****************************************************************************/
/* Глобальные переменые                                                      */
/*****************************************************************************/
static GRand * fortune = NULL;
/*****************************************************************************/
/* Вспомогательные функция                                                   */
/*****************************************************************************/
static int generate_number_card(void)
{
	return g_rand_int_range(fortune,0,(AMOUNT_SUIT_CARD *AMOUNT_VALUE_CARD));
}

static uint8_t 
static uint8_t deck_card[AMOUNT_SUIT_CARD * AMOUNT_VALUE_CARD] = {0};

static int zero_deck_card(void)
{
	int len = AMOUNT_SUIT_CARD * AMOUNT_VALUE_CARD;
	uint8_t * d = deck_card;
	memset(d,NO,len);
	return SUCCESS;
}

static uint8_t check_deck_card(void)
{
	int i = 0;
	int c = 0;
	int number = generate_number_card();

	c = deck_card[number];
	if(c == NO){
		deck_card[number] = YES;
	}
	else{
		number++;
		for(i = 0;i < (AMOUNT_SUIT_CARD*AMOUNT_VALUE_CARD);i++,number++){
			c = deck_card[number];
			if(c == NO){
				deck_card[number] = YES;
				break;
			}
			if(number == (AMOUNT_SUIT_CARD*AMOUNT_VALUE_CARD) ){
				number = -1;
			}
		}
	}
	if(i == (AMOUNT_SUIT_CARD * AMOUNT_VALUE_CARD)){
		number = i;
	}
	return number;
}
/*****************************************************************************/
/* Основная функция                                                          */
/*****************************************************************************/

int generate_deck(deck_cards_s * psd)
{
	int i;
	int number; 
	uint8_t card;

	zero_deck_card();

	for(i = 0;i < (AMOUNT_CARD_PLAYER -1);i++){
		number 

		psd->player_center[i] = CARD(suit,value);

	}

	return SUCCESS;
}

int init_decks(void)
{
	fortune = g_rand_new();
	return SUCCESS;
}

int deinit_decks(void)
{
	g_rand_free(fortune);
	return SUCCESS;
}
/*****************************************************************************/
