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

#ifndef DECKS_H
#define DECKS_H

#include "decks_pub.h"

int generate_deck(deck_cards_s * psd);

int init_decks(void);
int deinit_decks(void);

#endif

/*****************************************************************************/
