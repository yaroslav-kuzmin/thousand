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
/* Структуры для обработки протокола взаимодействия между клиентом и сервером*/
/*****************************************************************************/

#ifndef PROTOCOL_H
#define PROTOCOL_H


typedef struct _message_cmd_s message_cmd_s;
struct _message_cmd_s
{
	uint16_t number;
	uint16_t type;

	union
	{
	uint16_t len;
	uint16_t msg;
	};
}__attribute__((packed));

typedef struct _message_login_s message_login_s;
struct _message_login_s
{
	uint16_t number;
	uint16_t type;
	uint16_t len;

	uint8_t login[LEN_NAME_PLAYER];
}__attribute__((packed));

typedef struct _message_passwd_s message_passwd_s;
struct _message_passwd_s
{
	uint16_t number;
	uint16_t type;
	uint16_t len;

	uint8_t passwd[MD5_DIGEST_LENGTH];
}__attribute__((packed));

typedef struct _message_player_s message_player_s;
struct _message_player_s
{
	uint16_t number;
	uint16_t type;
	uint16_t len;

	uint8_t number_player;
	uint8_t name_player[LEN_NAME_PLAYER];
}__attribute__((packed));

typedef struct _message_status_player_s message_status_player_s;
struct _message_status_player_s
{
	uint16_t number;
	uint16_t type;
	uint16_t len;

	uint8_t number_player;
	uint8_t status_player;
}__attribute__((packed));

typedef struct _message_point_s message_point_s;
struct _message_point_s
{
	uint16_t number;
	uint16_t type;
	uint16_t len;

	uint8_t player;
	int16_t point;
	uint8_t bolt;
}__attribute__((packed));

typedef struct _message_acting_s message_acting_s;
struct _message_acting_s
{
	uint16_t number;
	uint16_t type;
	uint16_t len;

	uint16_t acting;
	uint8_t player;
}__attribute__((packed));

typedef struct _message_cards_s message_cards_s;
struct _message_cards_s
{
	uint16_t number;
	uint16_t type;
	uint16_t len;

	uint8_t amount;
	uint8_t card[AMOUNT_CARD_PLAYER];
}__attribute__((packed));

typedef struct _message_bets_s message_bets_s;
struct _message_bets_s
{
	uint16_t number;
	uint16_t type;
	uint16_t len;

	uint8_t player;
	uint16_t bets;
}__attribute__((packed));

typedef union _all_message_u all_message_u;
union _all_message_u
{
	message_cmd_s           cmd;
	message_login_s         login;
	message_passwd_s        passwd;
	message_player_s        player;
	message_point_s         point;
	message_acting_s        acting;
	message_status_player_s status_player;
	message_cards_s         cards;
	message_bets_s          bets;
};
/**************** type message ***********************************************/
#define CMD_CHECK_CONNECT           0x0001
#define MESSAGE_LOGIN               0x0002
#define MESSAGE_PASSWD              0x0003
#define CMD_ACCESS_DENIED_PASSWD    0x0004
#define CMD_ACCESS_DENIED_LOGIN     0x0005
#define CMD_ACCESS_ALLOWED          0x0006
#define CMD_NEW_ACTING              0x0007
#define CMD_JOIN_ACTING             0x0008
#define MESSAGE_JOIN_PLAYER         0x0009
#define CMD_GAME_OVER               0x000A
#define CMD_NUMBER_ROUND            0x000B
#define CMD_POINT                   0x000C
#define MESSAGE_STATUS_PLAYER       0x000D
#define MESSAGE_CARDS               0x000E
#define CMD_AUCTION                 0x000F
#define MESSAGE_BETS                0x0010

/********************* message ***********************************************/
#define END_NUMBER_ACTING   0

#endif

/******************************************************************************/

