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
/* Модуль deck  - содержит структуру                                         */
/*****************************************************************************/

#ifndef DECK_H
#define DECK_H

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
