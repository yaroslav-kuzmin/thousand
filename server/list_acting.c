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
/* Модуль acting                                */
/*****************************************************************************/

/*****************************************************************************/
/* Дополнительные файлы                                                      */
/*****************************************************************************/
#include <stdint.h>

#include <glib.h>

#include "pub.h"

#include "list_user_pub.h"
/*****************************************************************************/
/* Глобальные переменые                                                      */
/*****************************************************************************/
#define AMOUNT_PLAYER      4
typedef struct _acting_s acting_s;
struct _acting_s
{
	uint16_t number;
	user_s * player[AMOUNT_PLAYER];

};

acting_s * 
/*****************************************************************************/
/* Вспомогательные функция                                                   */
/*****************************************************************************/

/*****************************************************************************/
/* Основная функция                                                          */
/*****************************************************************************/
int init_acting(void)
{
	return SUCCESS;
}

int deinit_acting(void)
{
	return SUCCESS;
}
int new_actings(void)
{
	return SUCCESS;
}

int current_actings(void)
{
	return SUCCESS;
}
/*****************************************************************************/
