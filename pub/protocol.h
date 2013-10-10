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
/* Структуры для обработки протокола взаимодействия между клиентом и сервером*/
/*****************************************************************************/

#ifndef PROTOCOL_H
#define PROTOCOL_H

#define LEN_USER_NAME      64

typedef struct _message_cmd_s message_cmd_s;
typedef union _message_cmd_u message_cmd_u;
struct _message_cmd_s
{
	uint16_t number;
	uint16_t type;
	uint16_t len;
}__attribute__((packed));
union _message_cmd_u
{
	message_cmd_s field;
	uint8_t array[sizeof(message_cmd_s)];
};

typedef struct _message_login_s message_login_s;
typedef union _message_login_u message_login_u;
struct _message_login_s
{
	uint16_t number;
	uint16_t type;
	uint16_t len;
	uint8_t login[LEN_USER_NAME];
}__attribute__((packed));

union _message_login_u
{
	message_login_s field;
	uint8_t array[sizeof(message_login_s)];
};

typedef struct _message_passwd_s message_passwd_s;
typedef union _message_passwd_u message_passwd_u;
struct _message_passwd_s
{
	uint16_t number;
	uint16_t type;
	uint16_t len;
	uint8_t passwd[MD5_DIGEST_LENGTH];
}__attribute__((packed));
union _message_passwd_u
{
	message_passwd_s field;
	uint8_t array[sizeof(message_passwd_s)];
};

typedef union _all_message_u all_message_u;
union _all_message_u
{
	message_cmd_u    cmd;
	message_login_u  login;
	message_passwd_u passwd;
};

#define CMD_CHECK_CONNECT    0x0001
#define MESSAGE_LOGIN        0x0002
#define MESSAGE_PASSWD       0x0003

#endif

/*****************************************************************************/

