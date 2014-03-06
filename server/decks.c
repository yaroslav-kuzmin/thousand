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
#include <stdlib.h>
#include <string.h>

#include <glib.h>

#include "pub.h"
#include "warning.h"
#include "log.h"
#include "cards.h"
#include "decks_pub.h"

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

static int deck_card[AMOUNT_SUIT_CARD * AMOUNT_VALUE_CARD] = {0};

static int zero_deck_card(void)
{
	int len = AMOUNT_SUIT_CARD * AMOUNT_VALUE_CARD;
	len = len * sizeof(int);
	int * d = deck_card;
	memset(d,NO,len);
	return SUCCESS;
}

static int check_deck_card(int card)
{
	int i = 0;
	int c = 0;
	unsigned int number = card;

	c = deck_card[number];
	if(c == NO){
		deck_card[number] = YES;
	}
	else{
		number++;
		for(i = 0;i < (AMOUNT_SUIT_CARD*AMOUNT_VALUE_CARD);i++,number++){
			if(number >= (AMOUNT_SUIT_CARD*AMOUNT_VALUE_CARD) ){
				number = 0;
			}
			c = deck_card[number];
			if(c == NO){
				deck_card[number] = YES;
				break;
			}
		}
	}
	if(i == (AMOUNT_SUIT_CARD * AMOUNT_VALUE_CARD)){
		number = i;
		global_warning("Вся колода занята!");
	}
	return number;
}

static uint8_t number_to_card(unsigned int number)
{
	uint8_t suit;
	uint8_t value;
	uint8_t card;

	if(number >= (AMOUNT_SUIT_CARD * AMOUNT_VALUE_CARD)){
		global_log("Неизвестная карта %d!",number);
		card = UNKNOWN_CARD;
		return card;
	}
	suit = number / AMOUNT_VALUE_CARD;
	switch(suit){
		case 0:
			card = HEARTS;
			break;
		case 1:
			card = DIAMONDS;
			break;
		case 2:
			card = CLUBS;
			break;
		case 3:
			card = SPADES;
			break;
		default :
			global_warning("Error SUIT CARD : %d",suit);
			break;
	}
	value = number - (suit * AMOUNT_VALUE_CARD);
	switch(value){
		case 0:
			card = CARD(card,ACE);
			break;
		case 1:
			card = CARD(card,TEN);
			break;
		case 2:
			card = CARD(card,KING);
			break;
		case 3:
			card = CARD(card,QUEEN);
			break;
		case 4:
			card = CARD(card,JACK);
			break;
		case 5:
			card = CARD(card,NINE);
			break;
		default :
			global_warning("Error VALUE CARD : %d",value);
			break;
	}
	return card;
}

static uint8_t generate_card(void)
{
	uint8_t card;
	int number = generate_number_card();
	number = check_deck_card(number);
	card = number_to_card(number);
	return card;
}

static int compare_card(const void *c1,const void *c2)
{
	uint8_t card1 = *((uint8_t*)c1);
	uint8_t card2 = *((uint8_t*)c2);
	if(card1 < card2 ){
		return COMPARE_MORE;
	}
	if(card1 > card2){
		return COMPARE_LESS;
	}
	return COMPARE_EQUALS;
}
static int sort_card(uint8_t ** cards,int size)
{
	uint8_t * c = *cards;
	qsort(c,size,sizeof(uint8_t),compare_card);
	return SUCCESS;
}
/*****************************************************************************/
/* Основная функция                                                          */
/*****************************************************************************/

int generate_deck(deck_cards_s * psd)
{
	int i;
	uint8_t card;
	uint8_t * pt;

	zero_deck_card();

	for(i = 0;i < (AMOUNT_CARD_PLAYER - 1);i++){
		card = generate_card();
		psd->dealing[PLAYER_CENTR][i] = card;
		card = generate_card();
		psd->dealing[PLAYER_LEFT][i] = card;
		card = generate_card();
		psd->dealing[PLAYER_RIGHT][i] = card;
	}
	psd->dealing[PLAYER_CENTR][i] = UNKNOWN_CARD;
	psd->dealing[PLAYER_LEFT][i] = UNKNOWN_CARD;
	psd->dealing[PLAYER_RIGHT][i] = UNKNOWN_CARD;
	for(i = 0;i < (AMOUNT_CARD_DEAL);i++){
		card = generate_card();
		psd->deal[i] = card;
	}
	pt = psd->dealing[PLAYER_CENTR];
	sort_card(&pt,(AMOUNT_CARD_PLAYER-1));
	pt = psd->dealing[PLAYER_LEFT];
	sort_card(&pt,(AMOUNT_CARD_PLAYER-1));
	pt = psd->dealing[PLAYER_RIGHT];
	sort_card(&pt,(AMOUNT_CARD_PLAYER-1));
	pt = psd->deal;
	sort_card(&pt,(AMOUNT_CARD_DEAL));

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
