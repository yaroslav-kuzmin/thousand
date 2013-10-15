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
/*  Email contact: esdt@mail.ru                                              */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************/
/* Модуль пользовательского интерфейса                                       */
/*****************************************************************************/

/*****************************************************************************/
/* Дополнительные файлы                                                      */
/*****************************************************************************/
#include <locale.h>
#include <ncurses.h>

#include "pub.h"
#include "total.h"
#include "warning.h"
#include "log.h"
/*****************************************************************************/
/* Глобальные переменые                                                      */
/*****************************************************************************/
#define MAIN_PAIR     1

#define MAX_WIDTH              50
#define MAX_HEIGHT             3

static WINDOW * main_win = NULL;
static int start_x = 0;
static int start_y = 0;
static char * str_locale = NULL;

/*****************************************************************************/
/* Вспомогательные функция                                                   */
/*****************************************************************************/
static int create_main_win(void)
{
	main_win = newwin(MAX_HEIGHT,MAX_WIDTH,start_y,start_x);
	wattron(main_win,COLOR_PAIR(MAIN_PAIR));
	box(main_win,0,0);
	wrefresh(main_win);

	return SUCCESS;
}
/*****************************************************************************/
/* Основная функция                                                          */
/*****************************************************************************/

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
	keypad(stdscr, TRUE);  /* разрешить преобразование кодов клавиатуры */
	nonl();         /* не делать NL->CR/NL при выводе */
	cbreak();       /* читать один символ за раз, не ждать \n */
	noecho();       /* не показывать ввод */

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

	start_y = (LINES - MAX_HEIGHT)/2;
	start_x = (COLS - MAX_WIDTH)/2;
	/**/
	box(stdscr,0,0);
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
