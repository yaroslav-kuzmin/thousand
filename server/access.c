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
/* Модуль проверяет доступ игроков к серверу                                 */
/*****************************************************************************/
/*****************************************************************************/
/* Дополнительные файлы                                                      */
/*****************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <openssl/md5.h>

#include <glib.h>

#include "pub.h"
#include "total.h"
#include "log.h"
#include "warning.h"
#include "protocol.h"

#include "list_user.h"
#include "list_message.h"
/*****************************************************************************/
/* Глобальные переменые                                                      */
/*****************************************************************************/
static int change_key_file = NO;
static GKeyFile * access_file = NULL;
/*****************************************************************************/
/* Вспомогательные функция                                                   */
/*****************************************************************************/

/*****************************************************************************/
/* Основная функция                                                          */
/*****************************************************************************/
int init_access_user(void)
{
	GError * check = NULL;
	gchar * file = NULL;

	file = get_access_file();

	access_file = g_key_file_new();

	g_key_file_load_from_file (access_file,file,G_KEY_FILE_KEEP_COMMENTS,&check);
	if(check != NULL){
		global_warning("Ощибка при обработки файла доступа %s : %s",file,check->message);
 		return FAILURE;
	}
	change_key_file = NO;
	global_log("Открыл файл доступа %s",file);
	return SUCCESS;
}

int deinit_access_user(void)
{
	FILE * stream = NULL;
	gchar * buffer_config = NULL;
	GError * check = NULL;
	gchar * file = NULL;

	file = get_access_file();

	if(change_key_file == YES){
		stream = fopen(file,"w");
		if(stream == NULL){
			global_warning("Немогу открыть файл доступа %s для записи !",file);
 			return FAILURE;
		}
		buffer_config = g_key_file_to_data(access_file,0,&check);
		if(check != NULL){
			global_warning("Несмог записать файл доступа %s : %s",file,check->message);
		}
		else{
 			fputs(buffer_config,stream);
			global_log("Записал файл доступа %s",file);
			g_free(buffer_config);
		}
		fclose(stream);
 	}
	global_log("Закрыл систему доступа на сервер!");
	return SUCCESS;
}

int access_user(void)
{
	user_s * ptu;
	int exit = FAILURE;
	uint8_t flag;
	int rc;
	uint32_t len;
	all_message_u * msg;

	ptu = get_first_user_list();
	for(;ptu != NULL;){
		flag = ptu->flag;
		if( !ACCESS_USER(flag)){
			if(MESSAGE_USER(flag)){
				len = sizeof(message_cmd_s);
				rc = read_message_list(ptu,(uint8_t**)&msg,len);
				if(rc == FAILURE){
					ptu = get_next_user_list();
					break;
				}
				/*DEBUG*/
				printf("number :> %d\n",msg->cmd.number);
				printf("type   :> %d\n",msg->cmd.type);
				printf("len    :> %d\n",msg->cmd.len); 			  	
			}
		}
		ptu = get_next_user_list();
	}

	return exit;
}
/*****************************************************************************/
