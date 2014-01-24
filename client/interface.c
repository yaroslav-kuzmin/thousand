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

#define MAX_WIDTH              80
#define MAX_HEIGHT             7

static WINDOW * log_win = NULL;
static char * str_locale = NULL;

/*****************************************************************************/
/* Вспомогательные функция                                                   */
/*****************************************************************************/
static object_s o_log_win;

static int draw_log_win(void)
{
	if(log_win != NULL){
		wrefresh(log_win);
	}
	return SUCCESS;
}
static int create_log_win(void)
{
	int y,x,w,h;

	y = (LINES - MAX_HEIGHT)/2;
	x = (COLS - MAX_WIDTH)/2;
	w = MAX_WIDTH;
	h = MAX_HEIGHT;
	log_win = newwin(h,w,y,x);
	keypad(log_win,TRUE);
	wattrset(log_win,COLOR_PAIR(MAIN_PAIR));
	box(log_win,0,0);
	o_log_win.y = y;
	o_log_win.x = x;
	o_log_win.h = h;
	o_log_win.w = w;
	o_log_win.data = NULL;

	draw_log_win();
	return SUCCESS;
}
/*****************************************************************************/
/* Основная функция                                                          */
/*****************************************************************************/
/******** Окно логина ****************/
static char * PLAYER = "player : ";
static char * PLAYER_FIELD = "__________";
object_s o_label_player;
object_s o_field_player;

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

	wmove(log_win,o_label_player.y,o_label_player.x);
	wprintw(log_win,o_label_player.data);
	wprintw(log_win,o_field_player.data);
	draw_log_win();

	return SUCCESS;
}
int if_get_name_player(char ** player,int len)
{
	int i,x,y;
	char *str = *player;
	chtype ch;

	wmove(log_win,o_field_player.y,o_field_player.x);
	for(i = 0;i < (len - 1);){
		ch = wgetch(log_win);
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
				getyx(log_win,y,x);
				wmove(log_win,y,x-1);
				waddch(log_win,ch);
				wmove(log_win,y,x-1);
			}
		}
		/*TODO добавить поддержку UTF-8*/
		if(isalnum(ch)){
			*(str+i) = (char)ch;
			i++;
			if(i < o_field_player.w){
				waddch(log_win,ch);
			}
		}
		draw_log_win();
	}

	str[i] = 0;

	return SUCCESS;
}
int if_set_name_player(char *player)
{
	int i;
	int len = strlen(player);
	wmove(log_win,o_field_player.y,o_field_player.x);
	for(i = 0;i< len;i++){
		waddch(log_win,player[i]);
	}
	draw_log_win();
	return SUCCESS;
}

/*************************************/
static char * PASSWD = "passwd : ";
static char * PASSWD_FIELD = "__________";
object_s o_label_passwd;
object_s o_field_passwd;

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

	wmove(log_win,o_label_passwd.y,o_label_passwd.x);
	wprintw(log_win,o_label_passwd.data);
	wprintw(log_win,o_field_passwd.data);
	draw_log_win();

	return SUCCESS;
}
int if_get_passwd(char ** passwd,int len)
{
	int i,y,x;
	char *str = *passwd;
	chtype ch;

	wmove(log_win,o_field_passwd.y,o_field_passwd.x);
	for(i = 0;i < (len - 1);){
		ch = wgetch(log_win);
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
				getyx(log_win,y,x);
				wmove(log_win,y,x-1);
				waddch(log_win,ch );
				wmove(log_win,y,x-1);
			}
		}
		/*TODO добавить поддержку UTF-8*/
		if(isalnum(ch)){
			*(str+i) = (char)ch;
			i++;
			if(i < o_field_passwd.w){
				waddch(log_win,ch );
			}
		}
		draw_log_win ();
	}

	str[i] = 0;

	return SUCCESS;
}
int if_set_passwd(char * passwd)
{
	int i;
	int len = strlen(passwd);
	wmove(log_win,o_field_passwd.y,o_field_passwd.x);
	for(i = 0;i< len;i++){
		waddch(log_win,passwd[i]);
	}
	draw_log_win();
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

	wmove(log_win,o_label_connect.y,o_label_connect.x);
	wprintw(log_win,o_label_connect.data);
	draw_log_win();
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
	wmove(log_win,o_label_connect.y,o_label_connect.x);
	wprintw(log_win,o_label_connect.data);
/*
	for(i = 0;i < len-1;i++){
		ch = *(str + i) | COLOR_PAIR(IN_MAIN_PAIR);
		waddch(log_win,ch);
	}
*/
	draw_log_win();
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

	wattrset(log_win,COLOR_PAIR(ERROR_PAIR));
	wmove(log_win,o_label_connect.y,o_label_connect.x);
	wprintw(log_win,"%s",str);
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
	wmove(log_win,y,x);
	wprintw(log_win,"%s",str);
	draw_log_win();
	for(;;){
	 	ch = wgetch(log_win);
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

	wmove(log_win,o_label_new_game.y,o_label_new_game.x);
	wprintw(log_win,"%s",o_label_new_game.data);
	draw_log_win();
	for(;;){
		ch = wgetch(log_win);
		if( ch == KEY_F(4)){
			rc = FAILURE;
			break;
		}
		if( (ch == CR) || (ch == TAB)){
			rc = SUCCESS;
			wprintw(log_win,"%s",o_label_create.data);
			draw_log_win();
			break;
		}
	}

	return rc;
}
/*************************************/
static char * GAME = "GAME : ";
object_s o_label_game;
int init_o_game(void)
{
	o_label_game.y = 2;
	o_label_game.x = 1;
	o_label_game.h = 1;
	o_label_game.w = strlen(GAME);
	o_label_game.data = GAME;

	return SUCCESS;
}
int if_create_game(uint16_t number)
{
	 chtype ch;
	wmove(log_win,o_label_game.y,o_label_game.x);
	wprintw(log_win,"%s0x%04x",o_label_game.data,number);
	if(number == 0){
		wprintw(log_win,"Not create game");
		for(;;){
		 	ch = wgetch(log_win);
			if( (ch == KEY_F(4)) || (ch == CR) ){
				break;
			}
		}
	}
	else{
		wmove(log_win,o_label_new_game.y,o_label_new_game.x);
		wprintw(log_win,"                ");
	}
	draw_log_win();

	return SUCCESS;
}
/*************************************/
/*отображение имен игроков           */
static char * PARTNER_LEFT  = "partner left  : ";
static char * PARTNER_RIGHT = "partner right : ";
object_s o_partner_left;
object_s o_partner_right;
int init_partner(void)
{
	o_partner_left.y = 3;
	o_partner_left.x = 1;
	o_partner_left.h = 1;
	o_partner_left.w = strlen(PARTNER_LEFT);
	o_partner_left.data = PARTNER_LEFT;

	o_partner_right.y = 4;
	o_partner_right.x = 1;
	o_partner_right.h = 1;
	o_partner_right.w = strlen(PARTNER_RIGHT);
	o_partner_right.data = PARTNER_RIGHT;

	return SUCCESS;
}

int if_partner_left(char * name)
{
	wmove(log_win,o_partner_left.y,o_partner_left.x);
	wprintw(log_win,"%s%s",o_partner_left.data,name);
	draw_log_win();
	return SUCCESS;
}
int if_partner_right(char * name)
{
	wmove(log_win,o_partner_right.y,o_partner_right.x);
	wprintw(log_win,"%s%s",o_partner_right.data,name);
	draw_log_win();
	return SUCCESS;
}
/*************************************/
#define LAST_Y             MAX_HEIGHT-2
#define LAST_X             1
interface_cmd_e if_cmd(void)
{
	interface_cmd_e cmd;
	chtype ch;

	wmove(log_win,LAST_Y,LAST_X);
	ch = wgetch(log_win);
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
	/*rc = nodelay(log_win,bf);*/
	if(non == TRUE){
		wtimeout(log_win,1000);/*ожидание нажатия клавиши 1000 милисекунд*/
	}
	else{
		wtimeout(log_win,-1); /*Ожидать нажатие клавиши*/
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

	create_log_win();

	init_o_player();
	init_o_passwd();
	init_o_connect();
	init_o_new_game();
	init_o_game();
	init_partner();

	return SUCCESS;
}

int close_interface(void)
{
	endwin();
	return SUCCESS;
}
/*****************************************************************************/
