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

#include <ncurses.h>

#include "pub.h"
#include "total.h"
#include "warning.h"
#include "log.h"
/*****************************************************************************/
/* Глобальные переменые                                                      */
/*****************************************************************************/
#define MAIN_PAIR     1
#define NL            '\n'
#define CR            '\r'
typedef struct _object_s object_s;
struct _object_s
{
	int y;
	int x;
	int w;
	int h;
	char * data;
};

#define MAX_WIDTH              50
#define MAX_HEIGHT             3

static WINDOW * main_win = NULL;
static char * str_locale = NULL;

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
	wattron(main_win,COLOR_PAIR(MAIN_PAIR));
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


static char * PLAYER = "player : ";
static char * PLAYER_FIELD = "__________";
object_s o_label_user;
object_s o_field_user;

int init_o_user(void)
{
	o_label_user.y = 1;
	o_label_user.x = 1;
	o_label_user.h = 1;
	o_label_user.w = strlen(PLAYER);
	o_label_user.data = PLAYER;

	o_field_user.y = 1;
	o_field_user.x = o_label_user.w + 1;
	o_field_user.h = 1;
	o_field_user.w = strlen(PLAYER_FIELD);
	o_field_user.data = PLAYER_FIELD;

	wmove(main_win,o_label_user.y,o_label_user.x);
	wprintw(main_win,o_label_user.data);
	wprintw(main_win,o_field_user.data);
	draw_main_win();

	return SUCCESS;		
}
int if_get_name_user(char ** user,int len)
{
	int i;
	char *str = *user;
	chtype ch;

	wmove(main_win,o_field_user.y,o_field_user.x);
	for(i = 0;i < (len - 1);){
		ch = wgetch(main_win);
		if((ch == KEY_ENTER) || (ch == CR) || (ch == NL)){
			break;
		}
		/*TODO добавить поддержку UTF-8*/
		if(isalnum(ch)){
			*(str+i) = (char)ch;
			i++;
			if(i < o_field_user.w){
				waddch(main_win,ch);
				draw_main_win();
			}	
		}
	}

	str[i] = 0;	
	
	return SUCCESS;
}
int if_set_name_user(char *user)
{
	int i;
	int len = strlen(user);
	wmove(main_win,o_field_user.y,o_field_user.x);
	for(i = 0;i< len;i++){
		waddch(main_win,user[i]);
	}
	draw_main_win();
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
	o_label_passwd.x = o_field_user.x + o_field_user.w + 2;
	o_label_passwd.h = 1;
	o_label_passwd.w = strlen(PASSWD);
	o_label_passwd.data = PASSWD;

	o_field_passwd.y = 1;
	o_field_passwd.x = o_label_passwd.w + 1;
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
	return SUCCESS;
}
int if_set_passwd(char * passwd)
{
	return SUCCESS;
}
/*************************************/
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
	
	init_o_user();
	init_o_passwd();

	return SUCCESS;
}

int close_interface(void)
{
	endwin();
	return SUCCESS;
}
/*****************************************************************************/
