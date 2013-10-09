/*****************************************************************************/
/*                                                                           */
/* Thousand - card game                                                      */
/*  Copyright (C) <2013> <Kuzmin Yaroslav>                                   */
/*                                                                           */
/*  This program is free software: you can redistribute it and/or modify     */
/*  it under the terms of the GNU General Public License as published by     */
/*  the Free Software Foundation, either version 3 of the License, or        */
/*  (at your option) any later version.                                      */
/*                                                                           */
/*  This program is distributed in the hope that it will be useful,          */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of           */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            */
/*  GNU General Public License for more details.                             */
/*                                                                           */
/*  You should have received a copy of the GNU General Public License        */
/*  along with this program.  If not, see <http://www.gnu.org/licenses/>.    */
/*                                                                           */
/*   Email contact: esdt@mail.ru                                             */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************/
/*  Модуль: Работы с файлом параметров                                       */
/*            инициализируем глобальную переменую                            */
/*****************************************************************************/

/*****************************************************************************/
/* Дополнительные файлы                                                      */
/*****************************************************************************/
#include <stdio.h>

#include <glib.h>

#include "pub.h"
#include "total.h"
#include "log.h"
#include "warning.h"

/*****************************************************************************/
/* глобальные переменые                                                      */
/*****************************************************************************/
static GKeyFile * ini_file = NULL;
static int change_config = NO;
/*****************************************************************************/
/* Основные  функции                                                         */
/*****************************************************************************/
GKeyFile * get_key_file(void)
{
	return ini_file;
}

int change_key_file(void)
{
	change_config = YES;
	return change_config;
}

int read_config(void)
{
	GError * check = NULL;
	gchar * file_ini = NULL;

	file_ini = get_ini_file();

	ini_file = g_key_file_new();

	g_key_file_load_from_file (ini_file,file_ini,G_KEY_FILE_KEEP_COMMENTS,&check);
	if(check != NULL){
		global_warning("Ощибка при обработки файла конфигурации %s : %s",file_ini,check->message);
 		return FAILURE;
	}
	global_log("Открыл файл конфигурации %s",file_ini);
	
 	return SUCCESS;	
}

int close_config(void)
{
	FILE * stream = NULL;
	gchar * buffer_config = NULL;
	GError * check = NULL;
	gchar * file_ini = NULL;

	file_ini = get_ini_file();

	if(change_config == YES){
		stream = fopen(file_ini,"w");
		if(stream == NULL){
			global_warning("Немогу открыть файл конфигурации для записи !");
 			return FAILURE;
		}
		buffer_config = g_key_file_to_data(ini_file,0,&check);
		if(check != NULL){
			global_warning("Несмог записать файл конфигурации %s : %s",file_ini,check->message);
		}
		else{
 			fputs(buffer_config,stream);
			global_log("Записал файл конфигурации %s",file_ini);
			g_free(buffer_config);
		}
		fclose(stream);
 	}
	return SUCCESS;
}

/*****************************************************************************/

