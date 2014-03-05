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
/* Модуль пользовательского интерфейса                                       */
/*****************************************************************************/

/*****************************************************************************/
/* Дополнительные файлы                                                      */
/*****************************************************************************/
#include <locale.h>
#include <ctype.h>
#include <string.h>
#include <stdint.h>
#include <openssl/md5.h>

#include <ncurses.h>

#include "pub.h"
#include "total.h"
#include "warning.h"
#include "log.h"
#include "protocol.h"
#include "cards.h"

#include "interface_cmd.h"
/*****************************************************************************/
/* Глобальные переменые                                                      */
/*****************************************************************************/
#define MAIN_PAIR      1
#define IN_MAIN_PAIR   2
#define ERROR_PAIR     3
#define IN_ERROR_PAIR  4

#define NL            '\n'
#define CR            '\r'
#define TAB            0x09
typedef struct _object_s object_s;
struct _object_s
{
	int y;
	int x;
	int w;
	int h;
	char * data;
};

#define MAX_WIDTH              100
#define MAX_HEIGHT             25

static WINDOW * main_win = NULL;
static char * str_locale = NULL;

static char black_hearts[4]   = {0xe2,0x99,0xa5,0};
static char black_diamonds[4] = {0xe2,0x99,0xa6,0};
static char black_clubs[4]    = {0xe2,0x99,0xa3,0};
static char black_spades[4]   = {0xe2,0x99,0xa0,0};
#if 0
static char white_hearts[4]   = {0xe2,0x99,0xa1,0};
static char white_diamonds[4] = {0xe2,0x99,0xa2,0};
static char white_clubs[4]    = {0xe2,0x99,0xa7,0};
static char white_spades[4]   = {0xe2,0x99,0xa4,0};
#endif
static char ace[3] = " A";
static char ten[3] = "10";
static char king[3] = " K";
static char queen[3] = " Q";
static char jack[3] = " J";
static char nine[3] = " 9";
static char shirt_suit[4] = {0xe2,0x96,0x92,0};
static char shirt_value[5] = {0x20,0xe2,0x96,0x92,0};

static char top_line[16] = {0xe2,0x94,0x8c, 0xe2,0x94,0x80, 0xe2,0x94,0x80, 0xe2,0x94,0x80, 0xe2,0x94,0x90, 0};
static char top_line_left[7] = {0xe2,0x94,0x8c, 0xe2,0x94,0x80, 0};
static char top_line_right[10] = {0xe2,0x94,0x80, 0xe2,0x94,0x80, 0xe2,0x94,0x90, 0};
static char bottom_line[16] = {0xe2,0x94,0x94, 0xe2,0x94,0x80, 0xe2,0x94,0x80, 0xe2,0x94,0x80, 0xe2,0x94,0x98, 0};
static char bottom_line_left[7] = {0xe2,0x94,0x94, 0xe2,0x94,0x80, 0};
static char bottom_line_right[10] = {0xe2,0x94,0x80, 0xe2,0x94,0x80, 0xe2,0x94,0x98, 0};
static char v_line[4] = {0xe2,0x94,0x82, 0};
static char h_line[4] = {0xe2,0x94,0x80,0};

#define WIDTH_CARD       5
#define HEIGHT_CARD      3
/*****************************************************************************/
/* Вспомогательные функция                                                   */
/*****************************************************************************/
static object_s o_main_win;

static int draw_main_win(void)
{
	if(main_win != NULL){
		wrefresh(main_win);
	}
	return SUCCESS;
}
static int create_main_win(void)
{
	int y,x,w,h;

	y = (LINES - MAX_HEIGHT)/2;
	x = (COLS - MAX_WIDTH)/2;
	w = MAX_WIDTH;
	h = MAX_HEIGHT;
	main_win = newwin(h,w,y,x);
	keypad(main_win,TRUE);
	wattrset(main_win,COLOR_PAIR(MAIN_PAIR));
	box(main_win,0,0);
	o_main_win.y = y;
	o_main_win.x = x;
	o_main_win.h = h;
	o_main_win.w = w;
	o_main_win.data = NULL;

	draw_main_win();
	return SUCCESS;
}
/*****************************************************************************/
/* Основная функция                                                          */
/*****************************************************************************/
/******** Окно логина ****************/
static char * PLAYER = "player : ";
static char * PLAYER_FIELD = "__________";
static char * PASSWD = "passwd : ";
static char * PASSWD_FIELD = "__________";
object_s o_label_player;
object_s o_field_player;
object_s o_label_passwd;
object_s o_field_passwd;

int init_o_player(void)
{
	o_label_player.y = 1;
	o_label_player.x = 1;
	o_label_player.h = 1;
	o_label_player.w = strlen(PLAYER);
	o_label_player.data = PLAYER;

	o_field_player.y = 1;
	o_field_player.x = o_label_player.w + 1;
	o_field_player.h = 1;
	o_field_player.w = strlen(PLAYER_FIELD);
	o_field_player.data = PLAYER_FIELD;

	wmove(main_win,o_label_player.y,o_label_player.x);
	wprintw(main_win,o_label_player.data);
	wprintw(main_win,o_field_player.data);
	draw_main_win();

	return SUCCESS;
}
int if_get_name_player(char ** player,int len)
{
	int i,x,y;
	char *str = *player;
	chtype ch;

	wmove(main_win,o_field_player.y,o_field_player.x);
	for(i = 0;i < (len - 1);){
		ch = wgetch(main_win);
		if(  (ch == KEY_ENTER)
		  || (ch == CR)
		  || (ch == NL)
		  || (ch == TAB)){
			break;
		}
		if(ch == KEY_BACKSPACE){
			i--;
			*(str+i) = 0;
			ch = '_';
			if(i < o_field_player.w){
				getyx(main_win,y,x);
				wmove(main_win,y,x-1);
				waddch(main_win,ch);
				wmove(main_win,y,x-1);
			}
		}
		/*TODO добавить поддержку UTF-8*/
		if(isalnum(ch)){
			*(str+i) = (char)ch;
			i++;
			if(i < o_field_player.w){
				waddch(main_win,ch);
			}
		}
		draw_main_win();
	}

	str[i] = 0;

	return SUCCESS;
}
int if_set_name_player(char *player)
{
	int i;
	int len = strlen(player);
	wmove(main_win,o_field_player.y,o_field_player.x);
	for(i = 0;i< len;i++){
		waddch(main_win,player[i]);
	}
	draw_main_win();
	return SUCCESS;
}

/*************************************/

int init_o_passwd(void)
{
	o_label_passwd.y = 1;
	o_label_passwd.x = o_field_player.x + o_field_player.w + 2;
	o_label_passwd.h = 1;
	o_label_passwd.w = strlen(PASSWD);
	o_label_passwd.data = PASSWD;

	o_field_passwd.y = 1;
	o_field_passwd.x = o_label_passwd.x + o_label_passwd.w;
	o_field_passwd.h = 1;
	o_field_passwd.w = strlen(PASSWD_FIELD);
	o_field_passwd.data = PASSWD_FIELD;

	wmove(main_win,o_label_passwd.y,o_label_passwd.x);
	wprintw(main_win,o_label_passwd.data);
	wprintw(main_win,o_field_passwd.data);
	draw_main_win();

	return SUCCESS;
}
int if_get_passwd(char ** passwd,int len)
{
	int i,y,x;
	char *str = *passwd;
	chtype ch;

	wmove(main_win,o_field_passwd.y,o_field_passwd.x);
	for(i = 0;i < (len - 1);){
		ch = wgetch(main_win);
		if(  (ch == KEY_ENTER)
		  || (ch == CR)
		  || (ch == NL)
		  || (ch == TAB)){

			break;
		}
		if(ch == KEY_BACKSPACE){
			i--;
			*(str+i) = 0;
			ch = '_';
			if(i < o_field_passwd.w){
				getyx(main_win,y,x);
				wmove(main_win,y,x-1);
				waddch(main_win,ch );
				wmove(main_win,y,x-1);
			}
		}
		/*TODO добавить поддержку UTF-8*/
		if(isalnum(ch)){
			*(str+i) = (char)ch;
			i++;
			if(i < o_field_passwd.w){
				waddch(main_win,ch );
			}
		}
		draw_main_win ();
	}

	str[i] = 0;

	return SUCCESS;
}
int if_set_passwd(char * passwd)
{
	int i;
	int len = strlen(passwd);
	wmove(main_win,o_field_passwd.y,o_field_passwd.x);
	for(i = 0;i< len;i++){
		waddch(main_win,passwd[i]);
	}
	draw_main_win();
	return SUCCESS;
}

/*************************************/
static char * CONNECT = "connect ";
object_s o_label_connect;
int init_o_connect(void)
{
	o_label_connect.y = 1;
	o_label_connect.x = o_field_passwd.x + o_field_passwd.w + 10;
	o_label_connect.h = 1;
	o_label_connect.w = strlen(PASSWD);
	o_label_connect.data = CONNECT;

	wmove(main_win,o_label_connect.y,o_label_connect.x);
	wprintw(main_win,o_label_connect.data);
	draw_main_win();
	return SUCCESS;
}
int if_set_connect(void)
{
/*
	int i;
	int len = strlen(str);
	chtype ch;
	char * str = CONNECT;
*/
	wmove(main_win,o_label_connect.y,o_label_connect.x);
	wprintw(main_win,o_label_connect.data);
/*
	for(i = 0;i < len-1;i++){
		ch = *(str + i) | COLOR_PAIR(IN_MAIN_PAIR);
		waddch(main_win,ch);
	}
*/
	draw_main_win();
	return SUCCESS;
}

static char * NOT_CONNECT = "not connect";
static char * PASSWORD_INCORRECT = "password incorrect !!";
static char * LOGIN_INCORRECT = "login busy !!";

int if_not_set_connetc(int type)
{
	char * str = NOT_CONNECT;
	int len ;
	chtype ch;
	int y = 3;
	int x ;

	wattrset(main_win,COLOR_PAIR(ERROR_PAIR));
	wmove(main_win,o_label_connect.y,o_label_connect.x);
	wprintw(main_win,"%s",str);
	switch(type){
		case INCORRECT_LOGIN:
			str = LOGIN_INCORRECT;
			break;
		case INCORRECT_PASSWORD:
			str = PASSWORD_INCORRECT;
			break;
	}

	len = strlen(str);
	x = (MAX_WIDTH - len)/2;
	wmove(main_win,y,x);
	wprintw(main_win,"%s",str);
	draw_main_win();
	for(;;){
	 	ch = wgetch(main_win);
		if( (ch == KEY_F(4)) || (ch == CR) ){
			break;
		}
	}
	return SUCCESS;
}

/*************************************/
static char * NEW_GAME = " new game ";
static char * CREATE = "create ";
object_s o_label_new_game;
object_s o_label_create;
int init_o_new_game(void)
{
	int w = strlen(NEW_GAME);

	o_label_new_game.y = 2;
	o_label_new_game.x = (MAX_WIDTH - w)/2;
	o_label_new_game.h = 1;
	o_label_new_game.w = w;
	o_label_new_game.data = NEW_GAME;

	o_label_create.y = 2;
	o_label_create.x = o_label_new_game.x + w;
	o_label_create.h = 2;
	o_label_create.w = strlen(CREATE);
	o_label_create.data = CREATE;

	return SUCCESS;
}
int if_new_game(void)
{
	int rc;
	chtype ch;

	wmove(main_win,o_label_new_game.y,o_label_new_game.x);
	wprintw(main_win,"%s",o_label_new_game.data);
	draw_main_win();
	for(;;){
		ch = wgetch(main_win);
		if( ch == KEY_F(4)){
			rc = FAILURE;
			break;
		}
		if( (ch == CR) || (ch == TAB)){
			rc = SUCCESS;
			wprintw(main_win,"%s",o_label_create.data);
			draw_main_win();
			break;
		}
	}

	return rc;
}
/*************************************/
static char * GAME = "   GAME   ";
#define SIZE_GAME_LINE       (((MAX_WIDTH - 2) * SIZE_UTF8_SYMBOL_0800) +1)
static char game_line[SIZE_GAME_LINE] = {0};
object_s o_label_game;
int init_o_game(void)
{
	int i;
	for(i = 0;i < (SIZE_GAME_LINE-1);i++){
		char s = h_line[i % SIZE_UTF8_SYMBOL_0800];
		game_line[i] = s;
	}
	o_label_game.y = 2;
	o_label_game.x = 1;
	o_label_game.h = 1;
	o_label_game.w = strlen(GAME);
	o_label_game.data = game_line;

	return SUCCESS;
}
int if_create_game(uint16_t number)
{
	chtype ch;
	wmove(main_win,o_label_game.y,o_label_game.x);
	wprintw(main_win,"%s",o_label_game.data);
	if(number == 0){
		wprintw(main_win,"Not create game");
		for(;;){
		 	ch = wgetch(main_win);
			if( (ch == KEY_F(4)) || (ch == CR) ){
				break;
			}
		}
	}
	else{
		wmove(main_win,o_label_new_game.y,o_label_new_game.x);
		wprintw(main_win,GAME);
	}
	draw_main_win();

	return SUCCESS;
}
/*************************************/
/*отображение имен игроков           */
static char * PARTNER_LEFT  = "partner left  : ";
static char * PARTNER_RIGHT = "partner right : ";
static char * POINT = "point :> ";
static char * BOLT =  "bolt  :> ";
static char * DEALER =  "dealer! ";
static char * AUTOMAT = "automat!";
static char * FREE =    "free!   ";
static char * CLEAN_CARD_PARTNER = "                    ";

object_s o_partner_left;
object_s o_partner_left_point;
object_s o_partner_left_bolt;
object_s o_partner_left_status;
object_s o_partner_left_card;
object_s o_partner_right;
object_s o_partner_right_point;
object_s o_partner_right_bolt;
object_s o_partner_right_status;
object_s o_partner_right_card;

int init_o_partner(void)
{
	o_partner_left.y = 6;
	o_partner_left.x = 1;
	o_partner_left.h = 1;
	o_partner_left.w = strlen(PARTNER_LEFT);
	o_partner_left.data = PARTNER_LEFT;

	o_partner_left_point.y = 7;
	o_partner_left_point.x = 1;
	o_partner_left_point.h = 1;
	o_partner_left_point.w = strlen(POINT);
	o_partner_left_point.data = POINT;

	o_partner_left_bolt.y = 8;
	o_partner_left_bolt.x = 1;
	o_partner_left_bolt.h = 1;
	o_partner_left_bolt.w = strlen(BOLT);
	o_partner_left_bolt.data = BOLT;

	o_partner_left_status.y = 9;
	o_partner_left_status.x = 1;
	o_partner_left_status.h = 1;
	o_partner_left_status.w = strlen(DEALER);
	o_partner_left_status.data = DEALER;

	o_partner_left_card.y = 10;
	o_partner_left_card.x = 1;
	o_partner_left_card.h = HEIGHT_CARD;
	o_partner_left_card.w = WIDTH_CARD;
	o_partner_left_card.data = NULL;

	o_partner_right.y = 6;
	o_partner_right.x = MAX_WIDTH/2;
	o_partner_right.h = 1;
	o_partner_right.w = strlen(PARTNER_RIGHT);
	o_partner_right.data = PARTNER_RIGHT;

	o_partner_right_point.y = 7;
	o_partner_right_point.x = MAX_WIDTH/2;
	o_partner_right_point.h = 1;
	o_partner_right_point.w = strlen(POINT);
	o_partner_right_point.data = POINT;

	o_partner_right_bolt.y = 8;
	o_partner_right_bolt.x = MAX_WIDTH/2;
	o_partner_right_bolt.h = 1;
	o_partner_right_bolt.w = strlen(BOLT);
	o_partner_right_bolt.data = BOLT;

	o_partner_right_status.y = 9;
	o_partner_right_status.x = MAX_WIDTH/2;
	o_partner_right_status.h = 1;
	o_partner_right_status.w = strlen(DEALER);
	o_partner_right_status.data = DEALER;


	o_partner_right_card.y = 10;
	o_partner_right_card.x = MAX_WIDTH/2;
	o_partner_right_card.h = HEIGHT_CARD;
	o_partner_right_card.w = WIDTH_CARD;
	o_partner_right_card.data = NULL;

	return SUCCESS;
}

int if_partner_left(char * name)
{
	wmove(main_win,o_partner_left.y,o_partner_left.x);
	wprintw(main_win,"%s%s",o_partner_left.data,name);
	draw_main_win();
	return SUCCESS;
}
int if_partner_left_point(uint16_t point)
{
	wmove(main_win,o_partner_left_point.y,o_partner_left_point.x);
	wprintw(main_win,"%s%05d",o_partner_left_point.data,point);
	draw_main_win();
	return SUCCESS;
}
int if_partner_left_bolt(uint8_t bolt)
{
	wmove(main_win,o_partner_left_bolt.y,o_partner_left_bolt.x);
	wprintw(main_win,"%s%d",o_partner_left_bolt.data,bolt);
	draw_main_win();
	return SUCCESS;
}
int if_partner_left_status(status_player_e st)
{
	char * str;
	switch(st){
		case dealer_player:
			str = DEALER;
			break;
		case automat_player:
			str = AUTOMAT;
			break;
		case free_player:
			str = FREE;
			break;
		default :
			str = FREE;
			break;
	}
	wmove(main_win,o_partner_left_status.y,o_partner_left_status.x);
	wprintw(main_win,"%s",str);
	draw_main_win();
	return SUCCESS;
}
int if_partner_left_card(uint8_t card)
{
	int i;
	/*отчистеть */
	wmove(main_win,o_partner_left_card.y,o_partner_left_card.x);
	wprintw(main_win,"%s",CLEAN_CARD_PARTNER);
	wmove(main_win,o_partner_left_card.y+1,o_partner_left_card.x);
	wprintw(main_win,"%s",CLEAN_CARD_PARTNER);
	wmove(main_win,o_partner_left_card.y+2,o_partner_left_card.x);
	wprintw(main_win,"%s",CLEAN_CARD_PARTNER);
	if(card == 0){
		return SUCCESS;
	}

	if(card > AMOUNT_CARD_PARTNER){
		card = AMOUNT_CARD_PARTNER;
	}
	for(i = 0;i < card;i++ ){
		wmove(main_win,o_partner_left_card.y,o_partner_left_card.x+(i*2));
		wprintw(main_win,"%s",top_line_left);
		wmove(main_win,o_partner_left_card.y+1,o_partner_left_card.x+(i*2));
		wprintw(main_win,"%s%s",v_line,shirt_suit);
		wmove(main_win,o_partner_left_card.y+2,o_partner_left_card.x+(i*2));
		wprintw(main_win,"%s",bottom_line_left);
	}
	wmove(main_win,o_partner_left_card.y,o_partner_left_card.x+(i*2));
	wprintw(main_win,"%s",top_line_right);
	wmove(main_win,o_partner_left_card.y+1,o_partner_left_card.x+(i*2));
	wprintw(main_win,"%s%s",shirt_value,v_line);
	wmove(main_win,o_partner_left_card.y+2,o_partner_left_card.x+(i*2));
	wprintw(main_win,"%s",bottom_line_right);

	draw_main_win();

	return SUCCESS;
}

int if_partner_right(char * name)
{
	wmove(main_win,o_partner_right.y,o_partner_right.x);
	wprintw(main_win,"%s%s",o_partner_right.data,name);
	draw_main_win();
	return SUCCESS;
}
int if_partner_right_point(uint16_t point)
{
	wmove(main_win,o_partner_right_point.y,o_partner_right_point.x);
	wprintw(main_win,"%s%05d",o_partner_right_point.data,point);
	draw_main_win();
	return SUCCESS;
}
int if_partner_right_bolt(uint8_t bolt)
{
	wmove(main_win,o_partner_right_bolt.y,o_partner_right_bolt.x);
	wprintw(main_win,"%s%d",o_partner_right_bolt.data,bolt);
	draw_main_win();
	return SUCCESS;
}
int if_partner_right_status(status_player_e st)
{
	char * str;
	switch(st){
		case dealer_player:
			str = DEALER;
			break;
		case automat_player:
			str = AUTOMAT;
			break;
		case free_player:
			str = FREE;
			break;
		default :
			str = FREE;
			break;
	}
	wmove(main_win,o_partner_right_status.y,o_partner_right_status.x);
	wprintw(main_win,"%s",str);
	draw_main_win();
	return SUCCESS;
}
int if_partner_right_card(uint8_t card)
{
	int i;
	/*отчистеть */
	wmove(main_win,o_partner_right_card.y,o_partner_right_card.x);
	wprintw(main_win,"%s",CLEAN_CARD_PARTNER);
	wmove(main_win,o_partner_right_card.y+1,o_partner_right_card.x);
	wprintw(main_win,"%s",CLEAN_CARD_PARTNER);
	wmove(main_win,o_partner_right_card.y+2,o_partner_right_card.x);
	wprintw(main_win,"%s",CLEAN_CARD_PARTNER);
	if(card == 0){
		return SUCCESS;
	}
	if(card > AMOUNT_CARD_PARTNER){
		card = AMOUNT_CARD_PARTNER;
	}
	for(i = 0;i < card;i++ ){
		wmove(main_win,o_partner_right_card.y,o_partner_right_card.x+(i*2));
		wprintw(main_win,"%s",top_line_left);
		wmove(main_win,o_partner_right_card.y+1,o_partner_right_card.x+(i*2));
		wprintw(main_win,"%s%s",v_line,shirt_suit);
		wmove(main_win,o_partner_right_card.y+2,o_partner_right_card.x+(i*2));
		wprintw(main_win,"%s",bottom_line_left);
	}
	wmove(main_win,o_partner_right_card.y,o_partner_right_card.x+(i*2));
	wprintw(main_win,"%s",top_line_right);
	wmove(main_win,o_partner_right_card.y+1,o_partner_right_card.x+(i*2));
	wprintw(main_win,"%s%s",shirt_value,v_line);
	wmove(main_win,o_partner_right_card.y+2,o_partner_right_card.x+(i*2));
	wprintw(main_win,"%s",bottom_line_right);

	draw_main_win();

	return SUCCESS;
}
/*************************************/
/*отображение номера раунда          */
static char * ROUND = "round :> ";
#define SIZE_STR_ROUND   10
static char * STATUS_BEGIN_ROUND =  "status : begin!";
static char * STATUS_AUCTION_ROUND = "status : auction!";
static char * STATUS_PLAY_ROUND =    "status : play!";
static char * STATUS_END_ROUND =     "status : end!";

object_s o_round;
object_s o_status_round;
object_s o_point;
object_s o_bolt;
object_s o_status;

int init_o_round(void)
{
	o_round.y = 4;
	o_round.x = 1;
	o_round.h = 1;
	o_round.w = strlen(ROUND);
	o_round.data = ROUND;

	o_status_round.y = 4;
	o_status_round.x = o_round.w + SIZE_STR_ROUND;
	o_status_round.h = 1;
	o_status_round.w = strlen(STATUS_AUCTION_ROUND);
	o_status_round.data = STATUS_BEGIN_ROUND;

	o_point.y = 18;
	o_point.x = 1;
	o_point.h = 1;
	o_point.w = strlen(POINT);
	o_point.data = POINT;

	o_bolt.y = 19;
	o_bolt.x = 1;
	o_bolt.h = 1;
	o_bolt.w = strlen(BOLT);
	o_bolt.data = BOLT;

	o_status.y = 20;
	o_status.x = 1;
	o_status.h = 1;
	o_status.w = strlen(DEALER);
	o_status.data = DEALER;
	return SUCCESS;
}

int if_number_round(uint16_t round)
{
	wmove(main_win,o_round.y,o_round.x);
	wprintw(main_win,"%s%04d",o_round.data,round);
	draw_main_win();
	return SUCCESS;
}

int if_status_round(status_round_e sr)
{
	char * str;
	wmove(main_win,o_status_round.y,o_status_round.x);
	switch(sr){
		case begin_round:
			str = STATUS_BEGIN_ROUND;
			break;
		case auction_round:
			str = STATUS_AUCTION_ROUND;
			break;
		case play_round:
			str = STATUS_PLAY_ROUND;
			break;
		case end_round:
			str = STATUS_END_ROUND;
			break;
		default:
			str = STATUS_BEGIN_ROUND;
			break;
	}
	wprintw(main_win,"%s",str);
	return SUCCESS;
}

int if_player_point(uint16_t point)
{
	wmove(main_win,o_point.y,o_point.x);
	wprintw(main_win,"%s%05d",o_point.data,point);
	draw_main_win();
	return SUCCESS;
}
int if_player_bolt(uint8_t bolt)
{
	wmove(main_win,o_bolt.y,o_bolt.x);
	wprintw(main_win,"%s%d",o_bolt.data,bolt);
	draw_main_win();
	return SUCCESS;
}
int if_player_status(status_player_e st)
{
	char * str;
	switch(st){
		case dealer_player:
			str = DEALER;
			break;
		case automat_player:
			str = AUTOMAT;
			break;
		case free_player:
			str = FREE;
			break;
		default :
			str = FREE;
			break;
	}
	wmove(main_win,o_status.y,o_status.x);
	wprintw(main_win,"%s",str);
	draw_main_win();
	return SUCCESS;
}
/*************************************/
/* отображение стола раздачи         */
/* отображение карт на столе                 */
#define SIZE_TABLE     (3*WIDTH_CARD)
static char * TABLE = "table : ";
static char * CLEAN_TABLE = "               ";

object_s o_table;
object_s o_card_left;
object_s o_card_center;
object_s o_card_right;

int init_o_table(void)
{
	o_table.y = 14;
	o_table.x = 1;
	o_table.h = 1;
	o_table.w = strlen(TABLE);
	o_table.data = TABLE;

	o_card_left.y = 14;
	o_card_left.x = (MAX_WIDTH/2) - (SIZE_TABLE/2);
	o_card_left.h = WIDTH_CARD;
	o_card_left.w = HEIGHT_CARD;
	o_card_left.data = NULL;

	o_card_center.y = 14;
	o_card_center.x = (MAX_WIDTH/2) - (WIDTH_CARD/2);
	o_card_center.h = WIDTH_CARD;
	o_card_center.w = HEIGHT_CARD;
	o_card_center.data = NULL;

	o_card_right.y = 14;
	o_card_right.x = (MAX_WIDTH/2) + (WIDTH_CARD/2)+1;
	o_card_right.h = WIDTH_CARD;
	o_card_right.w = HEIGHT_CARD;
	o_card_right.data = NULL;

	return SUCCESS;
}

int if_table(void)
{
	wmove(main_win,o_table.y,o_table.x);
	wprintw(main_win,"%s",o_table.data);
	draw_main_win();
	return SUCCESS;
}
static char * check_suit(uint8_t suit)
{
	char * str = shirt_suit;
	switch(suit){
		case HEARTS:
			str = black_hearts;
			break;
		case DIAMONDS:
			str = black_diamonds;
			break;
		case CLUBS:
			str = black_clubs;
			break;
		case SPADES:
			str = black_spades;
			break;
	}
	return str;
}
static char * check_value(uint8_t value)
{
	char * str = shirt_value;
	switch(value){
		case ACE:
			str = ace;
			break;
		case TEN:
			str = ten;
			break;
		case KING:
			str = king;
			break;
		case QUEEN:
			str = queen;
			break;
		case JACK:
			str = jack;
			break;
		case NINE:
			str = nine;
			break;
	}
	return str;
}
int if_clean_table(void)
{
	wmove(main_win,o_card_left.y,o_card_left.x);
	wprintw(main_win,"%s",CLEAN_TABLE);
	wmove(main_win,(o_card_left.y + 1),o_card_left.x);
	wprintw(main_win,"%s",CLEAN_TABLE);
	wmove(main_win,(o_card_left.y + 2),o_card_left.x);
	wprintw(main_win,"%s",CLEAN_TABLE);

	return SUCCESS;
}
int if_table_card_left(uint8_t card)
{
	char * card_s = check_suit(SUIT_CARD(card));
	char * card_v = check_value(VALUE_CARD(card));

	wmove(main_win,o_card_left.y,o_card_left.x);
	wprintw(main_win,"%s",top_line);
	wmove(main_win,(o_card_left.y + 1),o_card_left.x);
	wprintw(main_win,"%s%s%s%s",v_line,card_s,card_v,v_line);
	wmove(main_win,(o_card_left.y + 2),o_card_left.x);
	wprintw(main_win,"%s",bottom_line);
	draw_main_win();

	return SUCCESS;
}
int if_table_card_center(uint8_t card)
{
	char * card_s = check_suit(SUIT_CARD(card));
	char * card_v = check_value(VALUE_CARD(card));

	wmove(main_win,o_card_center.y,o_card_center.x);
	wprintw(main_win,"%s",top_line);
	wmove(main_win,(o_card_center.y + 1),o_card_center.x);
	wprintw(main_win,"%s%s%s%s",v_line,card_s,card_v,v_line);
	wmove(main_win,(o_card_center.y + 2),o_card_center.x);
	wprintw(main_win,"%s",bottom_line);
	draw_main_win();

	return SUCCESS;
}
int if_table_card_right(uint8_t card)
{
	char * card_s = check_suit(SUIT_CARD(card));
	char * card_v = check_value(VALUE_CARD(card));

	wmove(main_win,o_card_right.y,o_card_right.x);
	wprintw(main_win,"%s",top_line);
	wmove(main_win,(o_card_right.y + 1),o_card_right.x);
	wprintw(main_win,"%s%s%s%s",v_line,card_s,card_v,v_line);
	wmove(main_win,(o_card_right.y + 2),o_card_right.x);
	wprintw(main_win,"%s",bottom_line);
	draw_main_win();

	return SUCCESS;
}
/*************************************/
/*  Отображение карт игрока          */
#define AMOUNT_CARD_PLAYER            10
static char * CLEAN_CARD_PLAYER = "     ";
object_s o_card[AMOUNT_CARD_PLAYER];

int init_card_player(void)
{
	int i;
	for(i = 0;i< AMOUNT_CARD_PLAYER;i++){
		o_card[i].y = 18;
		o_card[i].x = 20 + (i * (WIDTH_CARD+1));
		o_card[i].h = HEIGHT_CARD;
		o_card[i].w = WIDTH_CARD;
		o_card[i].data = NULL;
	}
	return SUCCESS;
}

int if_card_player(uint8_t number_card,uint8_t card,uint8_t select)
{
	int i;
	int x,y;
	char * card_s;
	char * card_v;

	if(number_card >= AMOUNT_CARD_PLAYER ){
		global_log("Неккоректный номер карты!");
		return SUCCESS;
	}
	x = o_card[number_card].x;
	y = o_card[number_card].y;

	for(i = 0;i< (HEIGHT_CARD*2);i++){
		wmove(main_win,y+i,x);
		wprintw(main_win,"%s",CLEAN_CARD_PLAYER);
	}

	if(select == NO){
		y += HEIGHT_CARD;
	}
	card_s = check_suit(SUIT_CARD(card));
	card_v = check_value(VALUE_CARD(card));

	wmove(main_win,y,x);
	wprintw(main_win,"%s",top_line);
	wmove(main_win,y+1,x);
	wprintw(main_win,"%s%s%s%s",v_line,card_s,card_v,v_line);
	wmove(main_win,y+2,x);
	wprintw(main_win,"%s",bottom_line);

	draw_main_win();
	return SUCCESS;
}
/*************************************/
#define LAST_Y             MAX_HEIGHT-2
#define LAST_X             1
interface_cmd_e if_cmd(void)
{
	interface_cmd_e cmd;
	chtype ch;

	wmove(main_win,LAST_Y,LAST_X);
	ch = wgetch(main_win);
	switch(ch){
		case KEY_F(4):
			cmd = exit_client;
			break;
		default:
			cmd = null_client;
			break;
	}
	return cmd;
}

int if_nonblock(int non)
{
	int rc = SUCCESS;
	/*rc = nodelay(main_win,bf);*/
	if(non == TRUE){
		wtimeout(main_win,1000);/*ожидание нажатия клавиши 1000 милисекунд*/
	}
	else{
		wtimeout(main_win,-1); /*Ожидать нажатие клавиши*/
	}
	return rc;
}

int init_interface(void)
{
	str_locale = setlocale(LC_CTYPE,LOCALE);
	if(str_locale == NULL){
		global_warning("Несмог поставить локаль %s!",LOCALE);
		return FAILURE;
	}
	global_log("Установлина локаль %s!",str_locale);

	/* инициализировать библиотеку curses */
	if(!(initscr())){
   	global_warning("ncurses failed\n\n");
		return FAILURE;
	}
	cbreak();       /* читать один символ за раз, не ждать \n */
	noecho();       /* не показывать ввод */
	keypad(stdscr,TRUE);  /* разрешить преобразование кодов клавиатуры */
	nonl();         /* не делать NL->CR/NL при выводе */
	/*raw();*/

	/* инициализация цветовой палитры */
	if (!has_colors()){
		endwin();
		printf("\nОшибка! Не поддерживаются цвета\n");
		return FAILURE;
	}
	start_color();
	init_pair(MAIN_PAIR, COLOR_GREEN, COLOR_BLACK);
	init_pair(IN_MAIN_PAIR, COLOR_BLACK, COLOR_GREEN);
	init_pair(ERROR_PAIR,COLOR_RED,COLOR_BLACK);
	init_pair(IN_ERROR_PAIR,COLOR_BLACK,COLOR_RED);
	bkgdset(COLOR_PAIR(MAIN_PAIR));
	attron(COLOR_PAIR(MAIN_PAIR));

	if( (LINES < MAX_HEIGHT) || (COLS < MAX_WIDTH)){
		global_warning("Маленький размер терминала %dx%d необходим %dx%d!"
		              ,COLS,LINES,MAX_WIDTH,MAX_HEIGHT);
		endwin();
		return FAILURE;
	}
	global_log("Размер терминала %dx%d!",COLS,LINES);

	/*Отрисовываем основное окно */
	box(stdscr,0,0);
	move(0,1);
	printw("Тысяча!");
	refresh();

	create_main_win();

	init_o_player();
	init_o_passwd();
	init_o_connect();
	init_o_new_game();
	init_o_game();
	init_o_round();
	init_o_partner();
	init_o_table();
	init_card_player();

	return SUCCESS;
}

int close_interface(void)
{
	endwin();
	return SUCCESS;
}
/*****************************************************************************/
