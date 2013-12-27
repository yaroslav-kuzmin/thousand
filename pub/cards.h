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
/* Модуль cards  работает с картами в игре                                   */
/*****************************************************************************/

#ifndef CARDS_H
#define CARDS_H

#define UNKNOWN_CARD       0xC0

#define AMOUNT_SUIT_CARD   4
#define HEARTS             0x30    /*Черви  U+2662 | U+2666*/
#define DIAMONDS           0x20    /*Бубны  U+2663 | U+2667*/
#define CLUBS              0x10    /*Крести U+2661 | U+2665*/
#define SPADES             0x00    /*Пики   U+2660 | U+2664*/

#define AMOUNT_VALUE_CARD  6
#define ACE                0x0B
#define TEN                0x0A
#define KING               0x04
#define QUEEN              0x03
#define JACK               0x02
#define NINE               0x00

#define CARD(suit,value)             (suit + value)
#define SUIT_CARD(card)              (card & 0xF0)
#define VALUE_CARD(card)             (card & 0x0F)

#define AMOUNT_CARD_PLAYER     8
#define AMOUNT_CARD_DEAL       3
typedef struct _deck_cards_s  deck_cards_s;
struct _deck_cards_s{
	uint8_t player_center[AMOUNT_CARD_PLAYER];
	uint8_t player_left[AMOUNT_CARD_PLAYER];
	uint8_t player_right[AMOUNT_CARD_PLAYER];
	uint8_t deal[AMOUNT_CARD_DEAL];
};

#endif

/*****************************************************************************/
