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
#include <ncurses.h>

#include "pub.h"
/*****************************************************************************/
/* Глобальные переменые                                                      */
/*****************************************************************************/

/*****************************************************************************/
/* Вспомогательные функция                                                   */
/*****************************************************************************/

/*****************************************************************************/
/* Основная функция                                                          */
/*****************************************************************************/
int init_interface(void)
{	
	initscr();      /* инициализировать библиотеку curses */
	keypad(stdscr, TRUE);  /* разрешить преобразование кодов клавиатуры */
	nonl();         /* не делать NL->CR/NL при выводе */
	/*cbreak();       [> читать один символ за раз, не ждать \n <]*/
	noecho();       /* не показывать ввод */

	/* инициализация цветовой палитры */
	if (!has_colors()){
		endwin();
		printf("\nОшибка! Не поддерживаются цвета\n");
		return FAILURE;
	}
	start_color();
	init_pair(1, COLOR_GREEN, COLOR_BLACK);

	return SUCCESS;
}

int close_interface(void)
{
	endwin();
	return SUCCESS;
}
/*****************************************************************************/
