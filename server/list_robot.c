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
/* Модуль запуска роботов для сервера                                        */
/*****************************************************************************/

/*****************************************************************************/
/* Дополнительные файлы                                                      */
/*****************************************************************************/
#include <stdint.h>

#include <glib.h>

#include "pub.h"
#include "log.h"

#include "access.h"

/*****************************************************************************/
/* Глобальные переменые                                                      */
/*****************************************************************************/
int number_robot = 0;
/*****************************************************************************/
/* Вспомогательные функция                                                   */
/*****************************************************************************/

/*****************************************************************************/
/* Основная функция                                                          */
/*****************************************************************************/
int run_robot(uint16_t acting)
{
	char * name = NULL; 
	char ** list = list_name_robot();
	int amount_robot = amount_name_robot();

	if(amount_robot == 0){
		global_log("Нет имен для создания робота!");
		return FAILURE;
	}

	if(number_robot == amount_robot){
		number_robot = 0;
	}
	list += number_robot;		
	name = *list;

	g_message("name robot :> %s : acting :> %#04x",name,acting);
	number_robot ++;

	return SUCCESS;
}
/*****************************************************************************/
