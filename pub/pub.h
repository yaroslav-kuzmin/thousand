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
/* Объявлене глобальных макросов                                             */
/*****************************************************************************/

#ifndef PUB_H
#define PUB_H
/*************** RETURN *************/
#define SUCCESS                0
#define FAILURE               -1

#define YES                    1
#define NO                     0


#define DELETE_ACTING_USER     2
#define DELETE_ACTING_SERVER   3

#define NOT_ACTING_DEL         NO
#define ACTING_DEl             YES

#define COMPARE_LESS          -1
#define COMPARE_MORE           1
#define COMPARE_EQUALS         0
/************************************/
#define BEGIN_TIME        1900

#define SERVER_FLAG       0x01
#define CLIENT_FLAG       0x02
#define ROBOT_FLAG        0x03
/************************************/
/***** Длина сообщений ****/
#define LEN_MESSAGE_BETS             3
#define LEN_MESSAGE_POINT            4
#define LEN_MESSAGE_STATUS_PLAYER    2
#define LEN_MESSAGE_ACTING           3
#define LEN_NAME_PLAYER              64                        /*длина строки имени игрока*/
#define LEN_MAX_MESSAGE              LEN_NAME_PLAYER + 1       /*длина максимального сообщения*/
/************************************/

#define SIZE_UTF8_SYMBOL_0080        2
#define SIZE_UTF8_SYMBOL_0800        3

#define WAITING_USER           100   /*Проверка подключения в секундах каждого пользователя на сервере*/

/************************************/
#define AMOUNT_PLAYER      3
#define PLAYER_CREATOR     0
#define PLAYER_CENTR       PLAYER_CREATOR
#define PLAYER_LEFT        1
#define PLAYER_RIGHT       2

#define AMOUNT_CARD_PLAYER     8
#define AMOUNT_CARD_DEAL       3
#define AMOUNT_CARD_PARTNER           AMOUNT_CARD_PLAYER
/************* СТАВКИ ***************/
#define MAX_BETS          300
#define AUTOMAT_BETS      100
#define DO_BETS           40
#define WAIT_BETS         30
#define PASS_BETS         20
#define NULL_BETS         10
/*************** ERROR **************/

#define NOT_CONNECT_SERVER            -10
#define NOT_CONNECT_CLIENT            -11
#define INCORRECT_CMD                 -12
#define INCORRECT_MESSAGE             -13
#define INCORRECT_LOGIN               -14
#define INCORRECT_PASSWORD            -15
#define ROBOT_CAN_NOT_CREATE_ACTING   -16
#define LIMIT_ACTING                  -17
#define LONG_DATA                     -18
#define INCORRECT_SIZE                -19
#define NOT_DATA_OBTAIN               -20
#endif


