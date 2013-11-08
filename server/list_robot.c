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
#include "total.h"
#include "log.h"
#include "ini.h"

#include "access.h"

/*****************************************************************************/
/* Глобальные переменые                                                      */
/*****************************************************************************/
static int number_robot = 0;
static GString * str_run_robot = NULL;
static gssize len_name_bin = 0;
static char * KEY_ROBOT = "robot";
/*****************************************************************************/
/* Вспомогательные функция                                                   */
/*****************************************************************************/

static char * get_name_bin(void)
{
	char * name = NULL;
	GKeyFile * ini_file = get_key_file();

	if(ini_file == NULL){
		return name;
	}

	name = g_key_file_get_string(ini_file,INI_SERVER_GROUP,KEY_ROBOT,NULL);
	if(name == NULL){
		global_log("Нет имени программы робота в файле ini в секции %s!",INI_SERVER_GROUP);
		return NULL;
	}
	global_log("Программа робот %s",name);
	
	return name;
}

/*****************************************************************************/
/* Основная функция                                                          */
/*****************************************************************************/
static char * OPTION_ONE = " -u ";
static char * OPTION_TWO = " -a ";
int run_robot(uint16_t acting)
{
	char * name_robot = NULL; 
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
	name_robot = *list;
	
	if(str_run_robot == NULL){
		char * name_bin = get_name_bin();
		if(name_bin == NULL){
			return FAILURE;
		}
		str_run_robot = g_string_new(name_bin);
		str_run_robot = g_string_append(str_run_robot,OPTION_ONE);
		len_name_bin = str_run_robot->len;
		g_free(name_bin);
	}
		
	str_run_robot = g_string_append(str_run_robot,name_robot);
	str_run_robot = g_string_append(str_run_robot,OPTION_TWO);
	g_string_append_printf(str_run_robot,"0x%04x",acting);

	global_log("Запуск робота :> %s",str_run_robot->str);


	number_robot ++;
	str_run_robot = g_string_truncate(str_run_robot,len_name_bin);
	g_message(" :> %s",str_run_robot->str);

	return SUCCESS;
}

int deinit_list_robot(void)
{
	g_string_free(str_run_robot,TRUE);
	return SUCCESS;
}
/*****************************************************************************/
