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
/* Дополнительные файлы                                                      */
/*****************************************************************************/
#include <time.h>

#include "pub.h"
#include "log.h"

/*****************************************************************************/
/* глобальные переменые                                                      */
/*****************************************************************************/
static int registration_operation = YES;

static unsigned long int date_current = 0;
/*****************************************************************************/
/* Вспомогательные функция                                                   */
/*****************************************************************************/

unsigned long int current_date(void)
{
	return date_current;
}

int init_current_date(void)
{
	struct tm * tm_current;
	time_t time_current = 0;

	int mday;
	int mon;
	int year;

	time_current = time(NULL);
	tm_current = gmtime(&time_current);

	mday = tm_current->tm_mday;
	mon = tm_current->tm_mon + 1;
	year = tm_current->tm_year + BEGIN_TIME;

	date_current = YEARtoTIME(year);
	date_current += MONtoTIME(mon);
	date_current += DAYtoTIME(mday);
	global_log("Current day : 0x%x",date_current);
	return SUCCESS;
}

unsigned long int min_date(void)
{
	int mday;
	int mon;
	int year;
	unsigned long int temp;

	mday = 1;
	mon = 1;
	year = BEGIN_TIME;

	temp = YEARtoTIME(year);
	temp += MONtoTIME(mon);
	temp += DAYtoTIME(mday);

	return temp;
}

/*****************************************************************************/
int get_registration_operation(void)
{
	return registration_operation;
}
void set_registration_operation(int rg)
{
	if(rg != NO){
		registration_operation = YES;
	}
	else{
		registration_operation = NO;
	}
}
/*****************************************************************************/


int check_config(void)
{

char *getenv(const char *name);
}

/*****************************************************************************/
