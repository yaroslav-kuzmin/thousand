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

#include <ncurses.h>

#include "pub.h"
#include "total.h"
#include "warning.h"
#include "log.h"
/*****************************************************************************/
/* Глобальные переменые                                                      */
/*****************************************************************************/
#define MAIN_PAIR     1
typedef _object_s object_s;
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

static int create_main_win(void)
{
	int y,x,w,h;
	memset(&o_main_win,0,sizeof(object_s));

	y = (LINES - MAX_HEIGHT)/2;
	x = (COLS - MAX_WIDTH)/2;
	w = MAX_WIDTH;
	h = MAX_HEIGHT;
	main_win = newwin(h,w,y,x);
	keypad(main_win,TRUE);
	wattron(main_win,COLOR_PAIR(MAIN_PAIR));
	box(main_win,0,0);
	wrefresh(main_win);
	o_main_win.y = y;
	o_main_win.x = x;
	o_main_win.h = h;
	o_main_win.w = w;
	o_main_win.data = NULL:	
	return SUCCESS;
}
/*****************************************************************************/
/* Основная функция                                                          */
/*****************************************************************************/


static char * PLAYER = "Игрок : ";
object_s o_user;
int init_o_user(void)
{
	
}

int if_get_name_user(char ** user,int len)
{
	int i;
	char *str = *user;
	chtype ch;
	int y,x;

	wmove(main_win,1,1);
	wprintw(main_win,PLAYER);
	wrefresh(main_win);
	for(i = 0;i < (len - 1);){
		ch = wgetch(main_win);
		if((ch == KEY_ENTER) || (ch == '\r') || (ch == '\n')){
			getyx(main_win,y,x);
			wmove(main_win,y,x+2);
			wrefresh(main_win);
			break;
		}
		/*TODO добавить поддержку UTF-8*/
		if(isalnum(ch)){
			*(str+i) = (char)ch;
			i++;
			if(i < SIZE_STR_USER){
				waddch(main_win,ch);
				wrefresh(main_win);
			}	
		}
	}

	str[i] = 0;	
	return SUCCESS;
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
	

	return SUCCESS;
}

int close_interface(void)
{
	endwin();
	return SUCCESS;
}
/*****************************************************************************/
