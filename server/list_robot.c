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
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>

#include <glib.h>

#include "pub.h"
#include "total.h"
#include "log.h"
#include "ini.h"
#include "warning.h"

#include "access.h"

/*****************************************************************************/
/* Глобальные переменые                                                      */
/*****************************************************************************/
static char * KEY_ROBOT = "robot";
static int number_robot = 0;
#define PATH_ROBOT        0
#define NAME_ROBOT        1
#define ACTING_ROBOT      2
static char * arg_list[] =
{
	NULL, /*полный путь к программе*/
	NULL, /*-u имя робота*/
	NULL, /*-a номер игры*/
	NULL
};
#define OPTION_ONE        "--user=%s"
#define SIZE_OPTION_ONE   30     /*"--user=robot00000000000"*/
#define OPTION_TWO        "--acting=0x%04X"
#define SIZE_OPTION_TWO   20     /*"--acting=0xFFFF"*/
static GString * option_one = NULL;
static GString * option_two = NULL;
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

static int spawn_robot(char * robot,char ** list)
{
	pid_t child_pid;

	child_pid = fork();
	if(child_pid != 0){
		global_log("Запуск робота : %d :> %s %s %s",child_pid,robot,list[NAME_ROBOT],list[ACTING_ROBOT]);
		return SUCCESS;
	}
	else{
		execv(robot,list);
		global_warning("Несмог запустить робота : %s!",robot);
		exit(0);
	}

	return FAILURE;
}

/*****************************************************************************/
/* Основная функция                                                          */
/*****************************************************************************/
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

	g_string_printf(option_one,OPTION_ONE,name_robot);
	g_string_printf(option_two,OPTION_TWO,acting);

	arg_list[NAME_ROBOT] = option_one->str;
	arg_list[ACTING_ROBOT] = option_two->str;

	number_robot ++;

	spawn_robot(arg_list[PATH_ROBOT],arg_list);

	g_message("run robot :> %s %s %s",arg_list[PATH_ROBOT],arg_list[NAME_ROBOT],arg_list[ACTING_ROBOT]);
	return SUCCESS;
}

int init_list_robot(void)
{
	char * name_bin = get_name_bin();
	if(name_bin == NULL){
		return FAILURE;
	}

	arg_list[PATH_ROBOT] = name_bin;
	option_one = g_string_sized_new(SIZE_OPTION_ONE);
	option_two = g_string_sized_new(SIZE_OPTION_TWO);

	return SUCCESS;
}

int deinit_list_robot(void)
{
	char * temp = arg_list[PATH_ROBOT];
	g_free(temp);
	g_string_free(option_one,TRUE);
	g_string_free(option_two,TRUE);
	return SUCCESS;
}
/*****************************************************************************/
