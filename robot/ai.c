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

#include <glib.h>

#include "pub.h"

/*****************************************************************************/
/* Глобальные переменые                                                      */
/*****************************************************************************/

uint8_t number_robot = 0;
#define MIN_LEVEL_MASTER  0
#define MAX_LEVEL_MASTER  5
int master_robot = MIN_LEVEL_MASTER; 
/*****************************************************************************/
/* Вспомогательные функция                                                   */
/*****************************************************************************/

/*****************************************************************************/
/* Основная функция                                                          */
/*****************************************************************************/
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


int ai_set_master_robot(char * str_master)
{
	int i = atoi(str_master);
	if( i < MIN_LEVEL_MASTER || i > MAX_LEVEL_MASTER ){
		master_robot = MIN_LEVEL_MASTER;
	}
	master_robot = i;
	return SUCCESS;
}
/*****************************************************************************/
