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
/*   Email contact: kuzmin.yaroslav@gmail.com                                */
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
#include <string.h>
#include <openssl/md5.h>

/*DEBUG*/
#include <stdio.h>

#include <glib.h>

#include "pub.h"
#include "total.h"
#include "log.h"
#include "warning.h"
#include "protocol.h"
#include "bit_flag.h"

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
static int full_login(user_s * psu)
{
	int rc;
	message_login_s * msg;
	char * n = psu->name;
	uint16_t len;
	uint32_t flag = psu->flag;

	rc = read_message_list(psu,(uint8_t**)&msg);
	if(rc < sizeof(message_cmd_s)){
		return FAILURE;
	}
	len = sizeof(message_cmd_s) + msg->len;
	if(rc < len){
		return FAILURE;
	}
	if(msg->type != MESSAGE_LOGIN){
		return FAILURE;
	}
	memcpy(n,msg->login,msg->len);
	del_message_list(psu,len);
	set_bit_flag(flag,login_user,1);
	global_log("На %d соединении игрок %s",psu->fd,psu->name);	
	return SUCCESS;
}

static int full_passwd(user_s * psu)
{
	int rc; 
	message_passwd_s * msg;
	uint8_t * p = psu->passwd;
	uint32_t flag = psu->flag;
	int len;

	rc = read_message_list(psu,(uint8_t**)&msg);
	if(rc < sizeof(message_passwd_s)){
		return FAILURE;
	}
	if(msg->type != MESSAGE_PASSWD){
		return FAILURE;
	}
	len = sizeof(message_passwd_s);
	memcpy(p,msg->passwd,msg->len);
	del_message_list(psu,len);
	set_bit_flag(flag,passwd_user,1);
	global_log("На %d соединении пароль ***",psu->fd);	
	return SUCCESS;
}
static char dig2sym[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
static char str_passwd[MD5_DIGEST_LENGTH * 2] = {0}; 
static char USER_GROUP[] = "user";

static int convert_passwd(uint8_t * p)
{
	uint8_t t;
	uint8_t tt;
	int i,j;

	for(i = 0,j =0;i < MD5_DIGEST_LENGTH;i++,j++){
		t = *(p+i);
		tt = ((t & 0xF0)>>4);
		str_passwd[j] = dig2sym[tt];
		j++;
		tt = (t & 0x0F);
		str_passwd[j] = dig2sym[tt];
	}
	return SUCCESS;
}

static int check_access(user_s * psu)
{
	GError *error = NULL;
	uint8_t * p = psu->passwd;
	char * n = psu->name;
	gchar * check_passwd;
	uint32_t flag = psu->flag;
	int rc;

	g_message("check access :> MD5");
	convert_passwd(p);

	check_passwd = g_key_file_get_string(access_file,USER_GROUP,n,&error);
	if(error == NULL){
		g_message("name         :> %s",n);
		g_message("str_passwd   :> %s",str_passwd);
		g_message("check_passwd :> %s",check_passwd);
		rc = strncmp(str_passwd,check_passwd,(MD5_DIGEST_LENGTH *2));
		if( rc == 0){
			/*пароль совпадает*/
			set_bit_flag(flag,access_server_user,1);
			global_log("Доступ разрешен на сервер игроку %s",psu->name);
		}
		
	}

	return SUCCESS;
}
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
	int exit = 0;
	uint32_t flag;
	int rc;

	ptu = get_first_user_list();

	for(;ptu != NULL;){
	 	flag = ptu->flag;
		rc = check_bit_flag(flag,access_server_user,1);
		if( rc == NO){
	 		rc = check_bit_flag(flag,message_user,1);
			if( rc == NO ){
				exit++;
			}
			else{	
	 			rc = check_bit_flag(flag,login_user,1);
				if( rc == NO){
					rc = full_login(ptu);
					if(rc == FAILURE){
						ptu = get_next_user_list();
	 		 			break;
	 		 		}
			 	}
				rc = check_bit_flag(flag,passwd_user,1);
				if(rc == NO){
					rc = full_passwd(ptu);
					if(rc == FAILURE){
						ptu = get_next_user_list();
						break;
	 				}
				}
				check_access(ptu);
			} 
		}
		ptu = get_next_user_list();
	}

	if(exit){
		exit = FAILURE;
	}
	else{
		exit = SUCCESS;
	}

	return exit; 
}
/*****************************************************************************/
