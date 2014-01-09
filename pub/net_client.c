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
/* Функции взаимодействия по сети                                            */
/*****************************************************************************/
/*****************************************************************************/
/* Дополнительные файлы                                                      */
/*****************************************************************************/
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <openssl/md5.h>

#include <glib.h>

#include "pub.h"
#include "total.h"
#include "warning.h"
#include "log.h"
#include "protocol.h"

/*****************************************************************************/
/* Глобальные переменые                                                      */
/*****************************************************************************/
static all_message_u pub_message;
static int fd_local_socket;
static uint16_t number_packed = 0;
static GByteArray * buff_save = NULL;
#define SIZE_READ_BUFF         1024
static uint8_t read_buff[SIZE_READ_BUFF] = {0};
/*****************************************************************************/
/* Вспомогательные функция                                                   */
/*****************************************************************************/

int init_socket(void)
{
	struct sockaddr_un ssa;
	char * local_socket;
	int rc;

	fd_local_socket = socket(PF_UNIX, SOCK_STREAM, 0);
	if(fd_local_socket == -1){
		global_warning("Немогу создать локальный сокет : %s", strerror(errno));
		return FAILURE;
	}
	global_log("Создали сокет %#x",fd_local_socket);
	ssa.sun_family = AF_LOCAL;
	local_socket = get_local_socket();
	strcpy(ssa.sun_path,local_socket);
	rc = connect(fd_local_socket,&ssa,SUN_LEN(&ssa));
	if(rc == -1){
		global_warning ("Несмог соединится с локальным сокетом : %s",ssa.sun_path);
		close(fd_local_socket);
		return FAILURE;
	}
	memset(&pub_message,0,sizeof(all_message_u));
	global_log("Соединились с сервером!");
	buff_save = g_byte_array_new();
	return SUCCESS;
}

int write_socket(uint8_t * buff,int len)
{
	int rc;
	rc = send(fd_local_socket, buff , len , 0);
	if(rc == -1){
		global_warning("Несмог отправить сообшение : %s",strerror(errno));
		rc = FAILURE;
	}
	else{
		rc = SUCCESS;
	}
	return rc;
}
static int wait_read_socket(void)
{
	int rc;
	rc = recv(fd_local_socket,read_buff,SIZE_READ_BUFF,0);
	if(rc == -1){
 		global_warning("Несмог прочитать сообшение : %d : %s",fd_local_socket,strerror(errno));
		rc = FAILURE;
	}
	else{
		buff_save = g_byte_array_append(buff_save,read_buff,rc);
		rc = SUCCESS;
	}

	return rc;
}
static int record_buff(uint8_t ** buff,int max_len)
{
 	uint8_t * dbuff = *buff;
	uint8_t * sbuff = NULL;
	message_cmd_s * cmd;
	uint16_t len = sizeof(message_cmd_s);

	if(len > buff_save->len){
		return FAILURE;
	}

	cmd = (message_cmd_s *)buff_save->data;
	switch(cmd->type){
		case CMD_NEW_ACTING:
		case CMD_JOIN_ACTING:
		case CMD_CHECK_CONNECT:
		case CMD_ACCESS_DENIED_LOGIN:
		case CMD_ACCESS_DENIED_PASSWD:
		case CMD_ACCESS_ALLOWED:
			break;
		default:
			len += cmd->len;
			break;
	}
	if(len > buff_save->len){
		return FAILURE;
	}

	if(len > max_len){
		/*TODO проверка на последовательность пакетов*/
		global_warning("Привышен размер данных %d > %d!",len,max_len);
		exit(EXIT_SUCCESS);
	}
	sbuff = buff_save->data;

	memcpy(dbuff,sbuff,len);
	buff_save = g_byte_array_remove_range(buff_save,0,len);

	return SUCCESS;
}

int read_socket(uint8_t ** buff,int max_len)
{
	int rc;

	for(;;){
		if(buff_save->len != 0){
			rc = record_buff(buff,max_len);
			if(rc == SUCCESS ){
				goto exit_read_socket;
			}
		}
		rc = wait_read_socket();
		if(rc == FAILURE){
			goto exit_read_socket;
		}
	}
exit_read_socket:

	return rc;
}

int close_socket(void)
{
	close(fd_local_socket);
	g_byte_array_free(buff_save,TRUE);
	return SUCCESS;
}

/*************************************/
int c_cmd_login(char * user)
{
	int rc;
	message_login_s msg;
	char * str = user;
	int len = strlen(str);
	uint8_t * str_msg = msg.login;

	msg.number = number_packed;
	msg.type = MESSAGE_LOGIN;
	memset(str_msg,0,LEN_NAME_PLAYER);
	if(len > LEN_NAME_PLAYER ){
		len = LEN_NAME_PLAYER;
	}
	msg.len = len;
	memcpy(str_msg,str,len);
	len = sizeof(message_cmd_s) + len;
	rc = write_socket((uint8_t*)&msg,len);
	if(rc == SUCCESS){
		number_packed ++;
		global_log("Отправил login %d : %d : %d : %s",msg.number,msg.type,msg.len,msg.login);
	}
	return rc;
}

int c_cmd_passwd(uint8_t * passwd)
{
	int rc;
	message_passwd_s msg;
	uint8_t * str = passwd;
	uint8_t * str_msg = msg.passwd;

	msg.number = number_packed;
	msg.type = MESSAGE_PASSWD;
	msg.len = MD5_DIGEST_LENGTH;
	memcpy(str_msg,str,MD5_DIGEST_LENGTH);
	rc = write_socket((uint8_t*)&msg,sizeof(message_passwd_s));
	if(rc == SUCCESS){
		number_packed ++;
		global_log("Отправил passwd %d : %d : %d ",msg.number,msg.type,msg.len);
	}

	return rc;
}

int c_answer_access_server(void)
{
	int rc;
	message_cmd_s * msg = (message_cmd_s*)&pub_message;

	rc = read_socket((uint8_t**)&msg,sizeof(message_cmd_s));
	if(rc == FAILURE){
	 	global_log("Нет связи с сервером!");
		rc = NOT_CONNECT_SERVER;
		return rc;
	}
	/*TODO проверка на неполный пакет */
	rc = FAILURE;
	switch(msg->type){
	 	case CMD_ACCESS_ALLOWED:
			rc =  SUCCESS;
			break;
		case CMD_ACCESS_DENIED_LOGIN:
			rc = INCORRECT_LOGIN;
			break;
		case CMD_ACCESS_DENIED_PASSWD:
			rc = INCORRECT_PASSWORD;
			break;
		default:
			rc  = INCORRECT_CMD;
			break;
	}

	 return rc;
}

int c_cmd_join_acting(uint16_t number)
{
	int rc;
	message_cmd_s cmd;
	cmd.number = number_packed;
	cmd.type = CMD_JOIN_ACTING;
	cmd.msg = number;

	rc = write_socket((uint8_t *)&cmd,sizeof(message_cmd_s));
	if(rc == SUCCESS){
		number_packed++;
		global_log("Отправил запрос на присоединение к игре %#x",number);
	}

	return rc;
}

int c_cmd_new_acting(void)
{
	int rc;
	message_cmd_s cmd;
	cmd.number = number_packed;
	cmd.type = CMD_NEW_ACTING;
	cmd.msg = 0;

	rc = write_socket((uint8_t *)&cmd,sizeof(message_cmd_s));
	if(rc == SUCCESS){
		number_packed++;
		global_log("Отправил запрос на создание игры ");
	}
	return rc;
}

int c_answer_new_acting(uint16_t * number)
{
	int rc;
	message_cmd_s * cmd = (message_cmd_s*)&pub_message;
	*number = 0;

	rc = read_socket((uint8_t**)&cmd,sizeof(message_cmd_s));
	if(rc == FAILURE){
		global_log("Нет связи с сервером!");
		rc = NOT_CONNECT_SERVER;
		return rc;
	}

	if(cmd->type != CMD_NEW_ACTING){
		rc =  INCORRECT_CMD;
		return rc;
	}

	*number = cmd->msg;

	return rc;
}

int c_answer_number_player(uint16_t * number)
{
	int rc;
	message_cmd_s * cmd = (message_cmd_s*)&pub_message;
	*number = 0;

	rc = read_socket((uint8_t**)&cmd,sizeof(message_cmd_s));
	if(rc == FAILURE){
		global_log("Нет связи с сервером!");
		rc = NOT_CONNECT_SERVER;
		return rc;
	}

	if(cmd->type != CMD_NUMBER_PLAYER){
		rc =  INCORRECT_CMD;
		return rc;
	}

	*number = cmd->msg;

	return rc;
}

int c_answer_join_acting(uint16_t * number)
{
	int rc;
	message_cmd_s * cmd = (message_cmd_s*)&pub_message;

	rc = read_socket((uint8_t**)&cmd,sizeof(message_cmd_s));
	if(rc == FAILURE){
		global_log("Нет связи с сервером!");
		rc = NOT_CONNECT_SERVER;
		return rc;
	}
	if(cmd->type != CMD_JOIN_ACTING){
		rc =  INCORRECT_CMD;
		return rc;
	}

	*number = cmd->msg;

	return SUCCESS;
}

int c_answer_name_partner(uint8_t * number,char ** name)
{
	int rc;
	message_player_s * msg = (message_player_s*)&pub_message;
	char * dname = *name;
	char * sname = (char *)msg->name_player;
	int len;

	rc = read_socket((uint8_t**)&msg,sizeof(message_player_s));
	if(rc == FAILURE){
		global_log("Нет связи с сервером!");
		rc = NOT_CONNECT_SERVER;
		return rc;
	}

	if(msg->type != MESSAGE_JOIN_PLAYER){
		rc = INCORRECT_MESSAGE;
		return rc;
	}
	len = msg->len;
	memcpy(dname,sname,len);
	*number = msg->number_player;
	return SUCCESS;
}

int c_cmd_game_over(uint16_t acting)
{
	int rc;
	message_cmd_s cmd;
	cmd.number = number_packed;
	cmd.type = CMD_GAME_OVER;
	cmd.msg = acting;

	rc = write_socket((uint8_t *)&cmd,sizeof(message_cmd_s));
	if(rc == SUCCESS){
		number_packed++;
		global_log("Отправил запрос на удаления игры : 0x%04x",acting);
	}
	return rc;
}

int c_answer_message(all_message_u * msg)
{
	int rc;
	rc = read_socket((uint8_t**)&msg,sizeof(all_message_u));
	if(rc == FAILURE){
		global_log("Нет связи с сервером!");
		rc = NOT_CONNECT_SERVER;
	}
	return rc;
}
/*****************************************************************************/
